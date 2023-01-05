#include "HTTPHandler.hpp"
#include <iterator>

namespace webserv {

/* HTTPHandler Class */
void HTTPHandler::handle(TCPSocket* socket, int fd) {
    HTTPHandler::impl m_impl(socket, fd);
}

/* HTTPHandler::impl Class */
HTTPHandler::impl::impl(TCPSocket* socket, int fd)
    : socket(socket), client(this->socket->connection(fd)) {

    std::string str = this->client->recv();
    std::cout << str << std::endl;
	
  	std::ifstream ifs("../webserv/website/dutesier.html");
	std::string p = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	std::cout << p << std::endl;
	std::cout << p.size() << std::endl;
    this->client->send("HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\nContent-Length: 1214\n\n" + p);
}

// HTTPHandler::impl::impl(TCPSocket* socket, int fd)
//     : socket(socket), client(this->socket->connection(fd)) {
// 	HTTPRequest req( this->client->recv() );
//  	this->client->send(HTTPResponse(req).to_str);
// }

} // namespace webserv
