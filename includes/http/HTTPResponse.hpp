#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "server/ServerConfig.hpp"

#define SP   std::string(" ")
#define CRLF std::string("\r\n")

namespace webserv {

struct HTTPResponse {

        /* Member Types */
        typedef std::map<std::string, std::string> HTTPHeader;
        typedef std::map<int, std::string> StatusMap;
        typedef std::pair<int, std::string>        status_t;

        /* Constructor and Destructor */
        HTTPResponse(void); // init == false
		HTTPResponse(int status_code, ServerConfig config);
        HTTPResponse(int code, HTTPHeader header, std::string body); // init == true
		~HTTPResponse(void);
        // HTTPResponse(HTTPResponse const& src);
        // HTTPResponse& operator=(HTTPResponse const& rhs);

        /* Other Methods */
        std::string to_str(void) const;
		std::map<int, std::string>	create_code_map( void ) const;
		std::map<std::string, std::string> create_mime_map(void) const;


        /* Member Attributes */
        bool        init;
		StatusMap	status_map;
        status_t    status;
        HTTPHeader  header;
        std::string body;
        const std::string version;
};

} // namespace webserv

#endif /* HTTP_RESPONSE_HPP */
