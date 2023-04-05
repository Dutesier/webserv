#include "http/HTTPRequest.hpp"

namespace webserv {

HTTPRequest::HTTPRequest() : m_statusCode(0) {}

HTTPRequest::HTTPRequest(std::string reqStr)
    : m_statusCode(0), m_reqStr(reqStr) {
    char*  buf;
    size_t end_pos = reqStr.find("\r\n\r\n");
    if (end_pos == std::string::npos) { throw(MalformedRequestException()); }

    std::istringstream iss(reqStr);
    std::string        start_line;
    getline(iss, start_line); // getting first line

    // handle headers
    std::string line;
    while (getline(iss, line) && line != "\r") {

        size_t pos = line.find(':');
        if (pos != std::string::npos) {

            if (!(buf = strtok(const_cast<char*>(line.c_str()), ":"))) {
                throw(MalformedRequestException());
            }
            std::string key(buf);
            if (!(buf = strtok(NULL, " \r"))) {
                throw(MalformedRequestException());
            }
            std::string val(buf);
            while ((buf = strtok(NULL, "\r"))) { val += std::string(buf); }
            m_headers[key] = val;
        }
    }

    // handle start line
    if (!(buf = strtok(const_cast<char*>(start_line.c_str()), " "))) {
        throw(MalformedRequestException());
    }

    std::string method = std::string(buf);
    if (method == "GET") { m_method = webserv::HTTPRequest::GET; }
    else if (method == "POST") { m_method = webserv::HTTPRequest::POST; }
    else if (method == "DELETE") { m_method = webserv::HTTPRequest::DELETE; }
    else { m_method = webserv::HTTPRequest::UNDEFINED; }

    if (!(buf = strtok(NULL, " "))) { throw(MalformedRequestException()); }
    // m_uri = smt::shared_ptr<Uri>(new Uri(std::string(buf)));
    m_resource = std::string(buf);

    if (!(buf = strtok(NULL, "\r"))) { throw(MalformedRequestException()); }
    m_version = std::string(buf);

    if ((buf = strtok(NULL, "\n"))) { throw(MalformedRequestException()); }

    // handle body
    if (reqStr.size() > end_pos + 4) { m_content = reqStr.substr(end_pos + 4); }
}

HTTPRequest::HTTPRequest(int statusCode) : m_statusCode(statusCode) {}

HTTPRequest::~HTTPRequest() {}

// returns string representation of request
std::string HTTPRequest::toStr(void) const { return (m_reqStr); }

// Set the request method
void HTTPRequest::setMethod(webserv::HTTPRequest::Method method) {
    m_method = method;
}

// Get the request method
webserv::HTTPRequest::Method HTTPRequest::getMethod() const { return m_method; }

// Set the request URL
void HTTPRequest::setResource(std::string const& resource) {
    m_resource = resource;
}

// Get the request URL
std::string const& HTTPRequest::getResource() const { return m_resource; }

// Set the version
void HTTPRequest::setVersion(std::string const& version) {
    m_version = version;
}

// Get the version
std::string const& HTTPRequest::getVersion() const { return m_version; }

// Set a request header
void HTTPRequest::setHeader(std::string const& name, std::string const& value) {
    m_headers[name] = value;
}

// Get a request header
std::string HTTPRequest::getHeader(std::string const& name) const {
    if (m_headers.find(name) != m_headers.end()) {
        return (m_headers.find(name)->second);
    }
    else { return (""); }
}

// Get all of the request headers as a string
std::string HTTPRequest::getAllHeaders() const {
    std::string output;

    std::map<std::string, std::string>::const_iterator it;
    for (it = m_headers.begin(); it != m_headers.end(); ++it) {
        output += it->first + ": " + it->second + "\r\n";
    }
    output += "\r\n";
    return output;
}

// Set the request body / content
void HTTPRequest::setContent(std::string const& content) {
    m_content = content;
}

// Get the request body / content
std::string const& HTTPRequest::getContent() const { return m_content; }

// Set the status code
void HTTPRequest::setStatusCode(int status) { m_statusCode = status; }

// Get the status code
int HTTPRequest::getStatusCode() const { return m_statusCode; }

// Get the "key=value" query values from URI
std::string HTTPRequest::getQueriesFromResource() {
    // std::vector<std::string> queries;
    std::string queryString;
    std::size_t queryStart = m_resource.find('?');

    if (queryStart == std::string::npos) { return queryString; }

    queryString = m_resource.substr(queryStart + 1);
    // splitting line into vector of strings
    // char* keyValuePair = strtok(const_cast<char*>(queryString.c_str()), "&");
    // while (keyValuePair) {
    //     queries.push_back(keyValuePair);
    //     keyValuePair = strtok(NULL, "/");
    // }

    return queryString;
}

std::string HTTPRequest::getScriptName() {
    if (this->isCGIRequest()) {
        std::string parsedURI = this->getRefinedResource();
        size_t      pos;

        if ((pos = parsedURI.find(".py")) != std::string::npos) {
            return (m_resource.substr(0, pos + 3));
        }
        else if ((pos = parsedURI.find(".cgi")) != std::string::npos) {
            return (m_resource.substr(0, pos + 4));
        }
        else if ((pos = parsedURI.find(".php")) != std::string::npos) {
            return (m_resource.substr(0, pos + 4));
        }
    }
    return "";
}

std::string HTTPRequest::getPathInfo() {
    if (this->isCGIRequest()) {
        std::string parsedURI = this->getRefinedResource();
        size_t      start;
        size_t      finish;

        finish = parsedURI.length();
        if ((start = parsedURI.find(".py")) != std::string::npos) {
            return (m_resource.substr(start + 3, finish - (start + 3)));
        }
        else if ((start = parsedURI.find(".cgi")) != std::string::npos) {
            return (m_resource.substr(start + 4, finish - (start + 4)));
        }
        else if ((start = parsedURI.find(".php")) != std::string::npos) {
            return (m_resource.substr(start + 4, finish - (start + 4)));
        }
    }
    return "";
}

std::string HTTPRequest::getRefinedResource() {
    std::size_t queryStart = m_resource.find('?');
    if (queryStart == std::string::npos) { return m_resource; }
    else { return m_resource.substr(0, queryStart); }
}

bool HTTPRequest::isCGIRequest() {
    std::string parsedURI = this->getRefinedResource();

    if (!parsedURI.empty()) {
        if (parsedURI.find(".py") == (parsedURI.length() - 3)) {
            return (true);
        }
        else if (parsedURI.find(".cgi") == (parsedURI.length() - 4)) {
            return (true);
        }
        else if (parsedURI.find(".php") == (parsedURI.length() - 4)) {
            return (true);
        }
        else if (parsedURI.find(".cgi/") != std::string::npos) {
            return (true);
        }
        else if (parsedURI.find(".py/") != std::string::npos) { return (true); }
        else if (parsedURI.find(".php/") != std::string::npos) {
            return (true);
        }
    }
    return (false);
}

bool HTTPRequest::isValid() const { return (!m_statusCode); }

std::ostream& operator<<(std::ostream&                       os,
                         webserv::HTTPRequest::Method const& me) {

    switch (me) {
        case webserv::HTTPRequest::UNDEFINED:
            return (os << std::string("UNDEFINED"));
        case webserv::HTTPRequest::GET: return (os << std::string("GET"));
        case webserv::HTTPRequest::POST: return (os << std::string("POST"));
        case webserv::HTTPRequest::DELETE: return (os << std::string("DELETE"));
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, HTTPRequest const& req) {
    std::string space = " ";
    os << req.getMethod() << space << req.getResource() << space
       << req.getVersion() << std::string("\r\n");
    os << req.getAllHeaders();
    os << req.getContent();
    return os;
}

char const* HTTPRequest::MalformedRequestException::what(void) const throw() {
    return ("HTTPRequest: malformed request");
}

} // namespace webserv
