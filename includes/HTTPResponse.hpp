#ifndef HTTPResponse
# define HTTPResponse

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

	HTTPResponse( void );
	~HTTPResponse( void ); /* Destructor */

	void	create_response(void);
	void	create_status_line(void);
	void	create_headers(void);
	void	create_body(void);

};

}

#endif /* HTTPResponse */

