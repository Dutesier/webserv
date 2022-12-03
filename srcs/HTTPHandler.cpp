#include "HTTPHandler.hpp"

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
    this->client->send(str);
}

// HTTPHandler::impl::impl(TCPSocket* socket, int fd)
//     : socket(socket), client(this->socket->connection(fd)) {
// 	HTTPRequest req( this->client->recv() );
//  	this->client->send(HTTPResponse(req).to_str);
// }

} // namespace webserv
