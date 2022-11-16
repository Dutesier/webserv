#include "SocketConnection.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

namespace webserv {

SocketConnection::SocketConnection(int fd, SocketAddress addr)
	: Socket(fd, addr, SOCK_STREAM) {}

SocketConnection::SocketConnection()
	: Socket() {}

SocketConnection::~SocketConnection(void) {
	this->close();
}


/* ************************************************************************** */
/* Other Functions                                                            */
/* ************************************************************************** */

bool	SocketConnection::close(void) {
	if (::close(this->fd) < 0) return (false);
	this->fd = -1;
	return (true);
}

/* ************************************************************************** */
/* I/O handling                                                               */
/* ************************************************************************** */

// TODO: find smarter ways to get buf
std::string	SocketConnection::recv(void) {
	char	buff[READING_BUFFER + 1];
	size_t bytes_read;
	std::string temp;

	bytes_read = ::recv(fd, &buff, READING_BUFFER, 0);
	if (bytes_read <= 0)
		return ("");
	buff[bytes_read] = '\0';
	temp += buff;
	
	return (temp);
}

<<<<<<< HEAD
bool	SocketConnection::send(std::string message) {
	return (::send(fd, message.c_str(), message.size(), 0) > 0);
=======
bool        SocketConnection::write_connection(std::string message){
	if (send(fd, message.c_str(), message.size(), 0) < 0) {
		return false;
	}
	return true;
>>>>>>> 572ed39563a7937eea1a4a862820ca0ab2d6db7c
}

} /* webserv */
