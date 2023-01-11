#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

#include <string>
#include <map>

namespace webserv {
    enum Method {
        UNDEFINED,
        GET,
        POST,
        DELETE
    };
    
} // namespace webserv

std::ostream& operator<<(std::ostream& os, const webserv::Method& me);

class HTTPRequest
{
public:
    // HTTP request methods
    HTTPRequest();
    HTTPRequest(int statusCode);
    ~HTTPRequest();

    // Set the request method
    void setMethod(webserv::Method method);

    // Get the request method
    webserv::Method getMethod() const;

    // Set the request URL
    void setResource(const std::string& resource);

    // Get the request URL
    const std::string& getResource() const;

    // Set the version
    void setVersion(const std::string& version);

    // Get the version
    const std::string& getVersion() const;

    // Set a request header
    void setHeader(const std::string& name, const std::string& value);

    // Get a request header
    std::string getHeader(const std::string& name) const;

    // Get all of the request headers as a string
    std::string getAllHeaders() const;

    // Set the request body / content
    void setContent(const std::string& content);

    // Get the request body / content
    const std::string& getContent() const;

    // Set the status code
    void setStatusCode(int status);

    // Get the status code
    int getStatusCode() const;

    // Check if there isnt a status code -> Valid
    bool isValid() const;

private:
    webserv::Method m_method;
    std::string m_resource;
    std::string m_version;
    std::map<std::string, std::string> m_headers;
    std::string m_content;
    int m_statusCode;
};

std::ostream& operator<<(std::ostream& os, const HTTPRequest& req);

#endif