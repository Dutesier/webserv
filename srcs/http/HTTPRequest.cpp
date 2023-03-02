#include "http/HTTPRequest.hpp"

HTTPRequest::HTTPRequest() : m_statusCode(0) {}

HTTPRequest::HTTPRequest(int statusCode) : m_statusCode(statusCode) {}

HTTPRequest::~HTTPRequest() {}

// Set the request method
void HTTPRequest::setMethod(webserv::Method method) { m_method = method; }

// Get the request method
webserv::Method HTTPRequest::getMethod() const { return m_method; }

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
        output += it->first + ":" + it->second + "\r\n";
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
std::string HTTPRequest::getQueriesFromResource(){
    // std::vector<std::string> queries;
    std::string queryString;
    std::size_t queryStart = m_resource.find('?');

    if (queryStart == std::string::npos) {
        return queryString;
    }

    queryString = m_resource.substr(queryStart+1);
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
        size_t pos;

        if ((pos = parsedURI.find(".py")) != std::string::npos){
            return (m_resource.substr(0, pos + 3));
        }
        else if ((pos = parsedURI.find(".cgi")) != std::string::npos) {
            return (m_resource.substr(0, pos + 4));
        }
        else if ((pos = parsedURI.find(".php")) != std::string::npos) {
            return (m_resource.substr(0, pos + 4));
        }
    }
}

std::string HTTPRequest::getPathInfo() {
    if (this->isCGIRequest()) {
        std::string parsedURI = this->getRefinedResource();
        size_t start;
        size_t finish;

        finish = parsedURI.find('?');
        if ((start = parsedURI.find(".py")) != std::string::npos){
            return (m_resource.substr(start + 3, finish - start - 1));
        }
        else if ((start = parsedURI.find(".cgi")) != std::string::npos) {
            return (m_resource.substr(start + 4, finish - start - 1));
        }
        else if ((start = parsedURI.find(".php")) != std::string::npos) {
            return (m_resource.substr(start + 4, finish - start - 1));
        }
    }
}


std::string HTTPRequest::getRefinedResource() {
    std::size_t queryStart = m_resource.find('?');
    if (queryStart == std::string::npos) {
        return m_resource;
    } else {
        return m_resource.substr(0, queryStart);
    }
}

bool HTTPRequest::isCGIRequest(){
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
    }
    return (false);
}

bool HTTPRequest::isValid() const { return (!m_statusCode); }

std::ostream& operator<<(std::ostream& os, webserv::Method const& me) {

    switch (me) {
        case webserv::UNDEFINED: return (os << std::string("UNDEFINED"));
        case webserv::GET: return (os << std::string("GET"));
        case webserv::POST: return (os << std::string("POST"));
        case webserv::DELETE: return (os << std::string("DELETE"));
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
