#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>

namespace webserv {

struct HTTPRequest {

        typedef enum e_method { GET, POST, DELETE } method_t;

        /* Constructor */
        HTTPRequest(std::string req);

        /* Member Attributes */
        method_t method;
};

} // namespace webserv

#endif /* HTTP_REQUEST_HPP */
