#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <map>
#include <sstream>
#include <string>

#define SP   std::string(" ")
#define CRLF std::string("\r\n")

namespace webserv {

struct HTTPResponse {

        /* Member Types */
        typedef std::map<std::string, std::string> HTTPHeader;
        typedef std::pair<int, std::string>        status_t;

        /* Constructor and Destructor */
        HTTPResponse(void); // init == false
        HTTPResponse(status_t status, HTTPHeader header, std::string body,
                     std::string version = "HTTP/1.1"); // init == true
        ~HTTPResponse(void);
        // HTTPResponse(HTTPResponse const& src);
        // HTTPResponse& operator=(HTTPResponse const& rhs);

        /* Other Methods */
        std::string to_str(void) const;

        /* Member Attributes */
        bool        init;
        status_t    status;
        HTTPHeader  header;
        std::string body;
        std::string version;
};

} // namespace webserv

#endif /* HTTP_RESPONSE_HPP */
