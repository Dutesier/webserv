#include "TCPSocket.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

namespace webserv {

TCPSocket::TCPSocket(int port, std::string host, int family)
	: Socket(port, host, family, SOCK_STREAM) {
	this->fd = socket(family, this->socktype, 0);
}

TCPSocket::~TCPSocket( void ) {

	typedef std::vector<SocketConnection *>::iterator iterator;

	this->close();
	for ( iterator it = this->connections.begin();
		  it < this->connections.end(); ++it) delete *it;
	this->connections.clear();
}


/* ************************************************************************** */
/* Other Functions (Socket setup)                                             */
/* ************************************************************************** */

// Binds a socket to a sockaddr structure and sets its flags
// In other words, gives an fd a data structure
bool	TCPSocket::bind(void) {
	return (!::bind(this->fd, this->addr.address(), this->addr.length()));
}

// This function sets socket's options
bool	TCPSocket::setsockopt(int level, int optname, const void *optval,
				  		   socklen_t optlen) {
	return (!::setsockopt(this->fd, level, optname, optval, optlen));
}

// Sets the port state to LISTEN and sets a BACKLOG max ammount of connections
// Every connection will have a specific fd/socket where we can communicate
bool	TCPSocket::listen(void) {
	return (!::listen(this->fd, BACKLOG));
}

bool	TCPSocket::shutdown(int how) {
	return (!::shutdown(this->fd, how));
}

bool	TCPSocket::close(void) {
	if (::close(this->fd) < 0) return (false);
	this->fd = -1;
	return (true);
}

// Starts accepting connections. Accept is a blocking call that will wait for
// an incoming connection we can set a RCVTIMEO (timeout) value for the socket
// in order to prevent accept from blocking our program
bool	TCPSocket::accept(void) {

	SocketAddress		connect_addr;
	SocketConnection*	sock_connect;

	int	connect_fd = ::accept(this->fd, connect_addr.address(),
						      connect_addr.length_ptr());
	if ( connect_fd < 0 ) return (false);
	sock_connect = new SocketConnection(connect_fd, connect_addr);
	this->connections.push_back(sock_connect);
	return (true);
}

/* ************************************************************************** */
/* Other Functions (I/O handling)                                             */
/* ************************************************************************** */

// TODO: i don't really like this connection == NULL verification. I think this
// will probably need to do something more specific in the future
std::string TCPSocket::recv(SocketConnection* connection) {
	/* The following if statement is useful for testing but might also be
	 * useful in prod */
	if (connection == NULL) {
		if (connections.size() > 0)
			connection = connections.at(0);
		else
			return ("No connection detected");
	}

	return (connection->recv());
}

bool TCPSocket::send(SocketConnection* connection, std::string response) {
	/* Same as above */
	if (connection == NULL){
		if (connections.size() > 0)
			connection = connections.at(0);
		else
			return ("No connection detected"); // or false maybe?
	}
	return (connection->send(response));
}

std::vector<SocketConnection*>	TCPSocket::get_connections(void) const {
	return (this->connections);
}

} /* webserv */
