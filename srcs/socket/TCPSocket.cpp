#include "socket/TCPSocket.hpp"

namespace webserv {

TCPSocket::TCPSocket(int port, std::string host, int family)
    : Socket(port, host, family, SOCK_STREAM) {
    m_fd = socket(family, m_socktype, 0);
}

TCPSocket::~TCPSocket(void) { close(); }

// Binds a socket to a sockaddr structure and sets its flags
// In other words, gives an fd a data structure
void TCPSocket::bind(void) {

    if (::bind(m_fd, m_addr.address(), m_addr.length()) != 0) {
        throw(BindFailureException());
	}
}

// This function sets socket's options
void TCPSocket::setsockopt(int level, int optname, const void* optval,
                           socklen_t optlen) {

    if (::setsockopt(m_fd, level, optname, optval, optlen)) {
        throw(SetOptFailureException());
	}
}

// Sets the port state to LISTEN and sets a BACKLOG max ammount of connections
// Every connection will have a specific fd/socket where we can communicate
void TCPSocket::listen(void) {
    if (::listen(m_fd, BACKLOG)) { throw(ListenFailureException()); }
}

void TCPSocket::shutdown(int how) {
    if (::shutdown(m_fd, how)) { throw(ShutdownFailureException()); }
}

void TCPSocket::close(void) {

    if (m_fd == -1) { return; }
    if (::close(m_fd) < 0) { throw(CloseFailureException()); }

    m_fd = -1;
}

// Starts accepting connections. Accept is a blocking call that will wait for
// an incoming connection we can set a RCVTIMEO (timeout) value for the socket
// in order to prevent accept from blocking our program
int TCPSocket::accept(void) {

    SocketAddress     connect_addr;

    int connect_fd =
        ::accept(m_fd, connect_addr.address(), connect_addr.length_ptr());

    if (connect_fd < 0) { throw(AcceptFailureException()); }

	smt::shared_ptr<SocketConnection> sock_connect(new SocketConnection(connect_fd, connect_addr));
	m_connection[connect_fd] = sock_connect;
    return (connect_fd);
}

// TODO: i don't really like this connection == NULL verification. I think this
// will probably need to do something more specific in the future
std::string TCPSocket::recv(int connect_fd) {

	std::map<int, smt::shared_ptr<SocketConnection> >::iterator it;
	it = m_connection.find(connect_fd);

	if (it == m_connection.end()) { throw (NoSuchConnectionException()); }

    return (m_connection[connect_fd]->recv());
}

void TCPSocket::send(int connect_fd, std::string response) {

	std::map<int, smt::shared_ptr<SocketConnection> >::iterator it;
	it = m_connection.find(connect_fd);

	if (it == m_connection.end()) { throw (NoSuchConnectionException()); }

    m_connection[connect_fd]->send(response);
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

char const* TCPSocket::NoSuchConnectionException::what(void) const throw() {
    return ("webserv::TCPSocket no such SocketConnection");
}

} // namespace webserv
