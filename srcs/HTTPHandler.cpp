#include "HTTPHandler.hpp"

namespace webserv {

/* HTTPHandler Class */
void HTTPHandler::handle(TCPSocket* socket, int fd) {
    HTTPHandler::impl m_impl(socket, fd);
}

/* HTTPHandler::impl Class */
HTTPHandler::impl::impl(TCPSocket* socket, int fd)
    : socket(socket), client(this->socket->connection(fd)) {

    smt::shared_ptr<HTTPRequest> req = this->client->recv();
    if (req != NULL) {
        std::cout << req->content << std::endl;
        // std::cout << *req << std::endl;
    }
    this->client->send("A string");
}

// HTTPHandler::impl::impl(TCPSocket* socket, int fd)
//     : socket(socket), client(this->socket->connection(fd)) {
// 	HTTPRequest req( this->client->recv() );
//  	this->client->send(HTTPResponse(req).to_str);
// }

} // namespace webserv
