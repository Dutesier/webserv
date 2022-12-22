#ifndef HTTP_Response
# define HTTP_Response

# include <map>
# include <iostream>
# include <fstream>
# include <sstream>

# include "HTTPHandler.hpp"

namespace webserv {

class HTTPResponse: protected HTTPHandler {

private:

	const std::map<std::string, std::string> mime_types;
	const std::map<int, std::string> response_code;

public:

	HTTPResponse(void );
	~HTTPResponse( void ); /* Destructor */

	void	create_response(void);
	void	create_status_line(void);
	void	create_headers(void);
	void	create_body(void);

	std::map<int, std::string>	create_code_map(void);
	std::map<std::string, std::string>	create_mime_map(void);


};

}

#endif /* HTTP_Response */

