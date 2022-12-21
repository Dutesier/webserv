#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

# include <map>

#include "TCPSocket.hpp"

namespace webserv {

class HTTPHandler {


    public:
		//HTTP response handling variables
		int									status_code;
		std::map<std::string, std::string>	headers;
		std::string							response_file;
		
		//HTTP response creation variables
		std::string	status_section;
		std::string	header_section;
		std::string	body_section;

		std::string	http_response;

		HTTPHandler();
		~HTTPHandler();


        static void handle(TCPSocket* socket, int fd);


#ifndef GTEST_TESTING

    private:

#endif

        /* PImpl Object */
        struct impl;

};

struct HTTPHandler::impl {
        impl(TCPSocket* socket, int fd);

        TCPSocket*        socket;
        SocketConnection* client;
};

struct HTTPResponse
{

};


} // namespace webserv

#endif /* HTTP_HANDLER_HPP */
