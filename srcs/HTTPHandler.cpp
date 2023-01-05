#include "HTTPHandler.hpp"

namespace webserv {

/* HTTPHandler Class */
void HTTPHandler::handle(TCPSocket* socket, int fd) {

    SocketConnection*   client = socket->connection(fd);
    HTTPHandler::m_impl impl(client->recv());

    try {
        impl.execute();
        client->send(impl.response->to_str());
    }
    catch ( std::exception& e ) {
        std::cout << e.what() << std::endl;
    }
}

/* HTTPHandler::impl Class */
HTTPHandler::m_impl::m_impl(std::string req) : request(new HTTPRequest(req)) {}

HTTPHandler::m_impl::~m_impl(void) {

    if ( this->request ) { delete this->request; }
    if ( this->response ) { delete this->response; }
}

void HTTPHandler::m_impl::execute(void) {

    // analyse this->request and understand what its requesting

    // do what request wants, like everything it wants 4 real

    // create a response
}

} // namespace webserv
