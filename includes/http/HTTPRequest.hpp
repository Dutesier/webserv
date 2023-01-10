#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <map>
#include <string>

namespace webserv {
enum Method { UNDEFINED, GET, POST, DELETE };
} // namespace webserv

class HTTPRequest {
    public:

		//Constructor
		HTTPRequest(std::string req);
		HTTPRequest();

        // HTTP request methods

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

        // Set the request body / content
        void setContent(const std::string& content);

        // Get the request body / content
        const std::string& getContent() const;

    private:

        webserv::Method                    m_method;
        std::string                        m_resource;
        std::string                        m_version;
        std::map<std::string, std::string> m_headers;
        std::string                        m_content;
};

#endif