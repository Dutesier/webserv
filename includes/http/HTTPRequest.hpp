#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "utils/Logger.hpp"

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace webserv {
enum Method { UNDEFINED, GET, POST, DELETE };

} // namespace webserv

std::ostream& operator<<(std::ostream& os, webserv::Method const& me);

class HTTPRequest {

    public:

        // HTTP request methods
        HTTPRequest(void);
        HTTPRequest(std::string req_str);
        HTTPRequest(int statusCode);
        ~HTTPRequest(void);

        // returns request in string format
        std::string toStr(void) const;

        // Set the request method
        void setMethod(webserv::Method method);

        // Get the request method
        webserv::Method getMethod() const;

        // Set the request URL
        void setResource(std::string const& resource);

        // Get the request URL
        std::string const& getResource() const;

        // Set the version
        void setVersion(std::string const& version);

        // Get the version
        std::string const& getVersion() const;

        // Set a request header
        void setHeader(std::string const& name, std::string const& value);

        // Get a request header
        std::string getHeader(std::string const& name) const;

        // Get all of the request headers as a string
        std::string getAllHeaders() const;

        // Set the request body / content
        void setContent(std::string const& content);

        // Get the request body / content
        std::string const& getContent() const;

        // Set the status code
        void setStatusCode(int status);

        // Get the status code
        int getStatusCode() const;

        // Get the "key=value" query values from URI
        std::string getQueriesFromResource();

        // Get the resource without query params
        std::string getRefinedResource();

        // Get the name of the script
        std::string getScriptName();

        // Get the extra info after the script name
        std::string getPathInfo();

        // Check if the request is a CGI attempt
        bool isCGIRequest();

        // Check if there isnt a status code -> Valid
        bool isValid() const;

        // MalformedRequestException exception
        struct MalformedRequestException : std::exception {
                char const* what(void) const throw();
        };

    private:

        webserv::Method                    m_method;
        std::string                        m_resource;
        std::string                        m_version;
        std::map<std::string, std::string> m_headers;
        std::string                        m_content;
        int                                m_statusCode;
        std::string                        m_req_str;
};

std::ostream& operator<<(std::ostream& os, HTTPRequest const& req);

#endif
