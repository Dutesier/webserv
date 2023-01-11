#include "HTTPHandler.hpp"

namespace webserv {

/* HTTPHandler Class */
void HTTPHandler::handle(TCPSocket* socket, int fd) {
    HTTPHandler::impl m_impl(socket, fd);
}

/* HTTPHandler::impl Class */
HTTPHandler::impl::impl(TCPSocket* socket, int fd)
    : socket(socket), client(this->socket->connection(fd)) {

    std::string req = this->client->recv();
    std::cout << req << std::endl;
    this->client->send("HTTP/1.1 404\r\nContent-Length: 1\r\n\r\nx");
}

// HTTPHandler::impl::impl(TCPSocket* socket, int fd)
//     : socket(socket), client(this->socket->connection(fd)) {
// 	HTTPRequest req( this->client->recv() );
//  	this->client->send(HTTPResponse(req).to_str);
// }

} // namespace webserv
