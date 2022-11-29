#include "TCPSocket.hpp"

namespace webserv {

TCPSocket::TCPSocket(int port, std::string host, int family)
    : Socket(port, host, family, SOCK_STREAM) {
    this->fd = socket(family, this->socktype, 0);
    FLOG_D("webserv::TCPSocket created a socket");
}

TCPSocket::~TCPSocket(void) {

    typedef std::vector<SocketConnection*>::iterator iterator;

    for (iterator it = this->connections.begin(); it < this->connections.end();
         ++it)
        delete *it;
    this->connections.clear();
    this->close();
    // FLOG_D("webserv::TCPSocket destroyed a socket");
}

// Binds a socket to a sockaddr structure and sets its flags
// In other words, gives an fd a data structure
void TCPSocket::bind(void) {
    if (::bind(this->fd, this->addr.address(), this->addr.length()) != 0)
        throw(BindFailureException());
    // FLOG_D("webserv::TCPSocket binded a socket on port");
}

// This function sets socket's options
void TCPSocket::setsockopt(int level, int optname, const void* optval,
                           socklen_t optlen) {
    if (::setsockopt(this->fd, level, optname, optval, optlen))
        throw(SetOptFailureException());
    // FLOG_D("webserv::TCPSocket set socket option optname");
}

// Sets the port state to LISTEN and sets a BACKLOG max ammount of connections
// Every connection will have a specific fd/socket where we can communicate
void TCPSocket::listen(void) {
    if (::listen(this->fd, BACKLOG)) throw(ListenFailureException());
    // FLOG_D("webserv::TCPSocket socket is now listenning on port");
}

void TCPSocket::shutdown(int how) {
    if (::shutdown(this->fd, how)) throw(ShutdownFailureException());
    // FLOG_D("webserv::TCPSocket shutdown socket");
}

void TCPSocket::close(void) {
    if (this->fd == -1) return;
    if (::close(this->fd) < 0) throw(CloseFailureException());
    this->fd = -1;
    FLOG_D("webserv::TCPSocket closed socket");
}

// Starts accepting connections. Accept is a blocking call that will wait for
// an incoming connection we can set a RCVTIMEO (timeout) value for the socket
// in order to prevent accept from blocking our program
void TCPSocket::accept(void) {

    SocketAddress     connect_addr;
    SocketConnection* sock_connect;

    int connect_fd =
        ::accept(this->fd, connect_addr.address(), connect_addr.length_ptr());
    if (connect_fd < 0) throw(AcceptFailureException());
    sock_connect = new SocketConnection(connect_fd, connect_addr);
    this->connections.push_back(sock_connect);
    // FLOG_D("webserv::TCPSocket accepted new client");
}

// TODO: i don't really like this connection == NULL verification. I think this
// will probably need to do something more specific in the future
std::string TCPSocket::recv(SocketConnection* connection) {
    /* The following if statement is useful for testing but might also be
     * useful in prod */
    if (connection == NULL) {
        if (connections.size() > 0) connection = connections.at(0);
        else throw(RecvFailureException());
    }
    return (connection->recv());
}

void TCPSocket::send(SocketConnection* connection, std::string response) {
    /* Same as above */
    if (connection == NULL) {
        if (connections.size() > 0) connection = connections.at(0);
        else throw(SendFailureException());
    }
    connection->send(response);
}

std::vector<SocketConnection*> TCPSocket::get_connections(void) const {
    return (this->connections);
}

char const* TCPSocket::BindFailureException::what(void) const throw() {
    return ("webserv::TCPSocket failure in bind()");
}

char const* TCPSocket::ListenFailureException::what(void) const throw() {
    return ("webserv::TCPSocket failure in listen()");
}

char const* TCPSocket::SetOptFailureException::what(void) const throw() {
    return ("webserv::TCPSocket failure in setsockopt()");
}

char const* TCPSocket::AcceptFailureException::what(void) const throw() {
    return ("webserv::TCPSocket failure in accept()");
}

char const* TCPSocket::ShutdownFailureException::what(void) const throw() {
    return ("webserv::TCPSocket failure in shutdow()");
}

char const* TCPSocket::CloseFailureException::what(void) const throw() {
    return ("webserv::TCPSocket failure in close()");
}

char const* TCPSocket::SendFailureException::what(void) const throw() {
    return ("webserv::TCPSocket failure in send()");
}

char const* TCPSocket::RecvFailureException::what(void) const throw() {
    return ("webserv::TCPSocket failure in recv()");
}

} // namespace webserv
