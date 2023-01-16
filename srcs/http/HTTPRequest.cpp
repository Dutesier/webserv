#include "http/HTTPRequest.hpp"

namespace webserv {

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

}
