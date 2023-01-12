#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include <map>
# include <string>
# include <sstream>

namespace webserv {

struct HTTPResponse {

	HTTPResponse( int code, std::map<std::string, std::string> header, std::string body = "", std::string version = "HTTP/1.1"); /* Default constructor */
	~HTTPResponse( void ); /* Destructor */

	std::string	to_str(void);


	int	m_status;
	std::string	m_reason;
	std::map<std::string, std::string> m_header;
	std::string m_body;
	std::string m_version;

	static std::map<int, std::string> s_status_map;
	static std::map<int, std::string>	create_status_map(void);
};
}
#endif /* HTTP_RESPONSE_HPP */