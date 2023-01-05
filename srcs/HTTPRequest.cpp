#include "HTTPRequest.hpp"

// Set the request method
void HTTPRequest::setMethod(webserv::Method method) {
    m_method = method;
}

// Get the request method
webserv::Method HTTPRequest::getMethod() const {
    return m_method;
}

// Set the request URL
void HTTPRequest::setResource(const std::string& resource) {
    m_resource = resource;
}

// Get the request URL
const std::string& HTTPRequest::getResource() const {
    return m_resource;
}

// Set the version
void HTTPRequest::setVersion(const std::string& version){
    m_version = version;
}

// Get the version
const std::string& HTTPRequest::getVersion() const {
    return m_version;
}

// Set a request header
void HTTPRequest::setHeader(const std::string& name, const std::string& value) {
    m_headers[name] = value;
}

// Get a request header
std::string HTTPRequest::getHeader(const std::string& name) const {
    if (m_headers.find(name) != m_headers.end()) {
        return m_headers.find(name)->second;
    } else {
        return NULL;
    }
}

// Set the request body / content
void HTTPRequest::setContent(const std::string& content) {
    m_content = content;
}

// Get the request body / content
const std::string& HTTPRequest::getContent() const {
    return m_content;
}