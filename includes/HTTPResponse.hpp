#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

#include <string>
#include <sstream>
#include <map>

#define SP std::string(" ")
#define CRLF std::string("\r\n")

namespace webserv {

struct HTTPResponse {

	/* Member Types */
	typedef std::map<std::string, std::string> HTTPHeader;
	typedef std::pair<int, std::string> status_t;

	/* Constructor and Destructor */
	HTTPResponse( status_t status, HTTPHeader header, std::string body,
				  std::string version = "HTTP/1.1" );
	~HTTPResponse( void );

	/* Other Methods */
	std::string to_str(void) const;

	/* Member Attributes */
	status_t status;
	HTTPHeader header;
	std::string body;
	std::string version;

};

} // webserv

#endif /* HTTP_RESPONSE_HPP */

