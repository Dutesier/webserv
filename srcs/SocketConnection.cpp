#include "SocketConnection.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

namespace webserv {

SocketConnection::SocketConnection(int fd, SocketAddress addr)
	: Socket(fd, addr, SOCK_STREAM) {}

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

bool	SocketConnection::send(std::string message) {
	return (::send(fd, message.c_str(), message.size(), 0) < 0);
}

// std::string SocketConnection::read_connection(void) {
// 	char buff[READING_BUFFER + 1]; // This will probably not be a fized size
// 	size_t bytes_read;
// 	std::string temp;
// 	//int maximum_iterations = 65535 / READING_BUFFER; // This is equivalent to having a 64Kb packet (TCP max packet size)
//
// 	// while ((bytes_read = recv(fd, &buff, READING_BUFFER, 0)) > 0) {
// 	// 	if (!maximum_iterations)
// 	// 		break;
// 	// 	buff[bytes_read] = '\0';
// 	// 	temp += buff;
// 	// 	--maximum_iterations;
// 	// 	if (bytes_read < READING_BUFFER)
// 	// 		break;
// 	// }
// 	// if (maximum_iterations == 65535 / READING_BUFFER) // No loop was done
// 		// return "No message in connection - RECV failed";
//
// 	bytes_read = recv(fd, &buff, READING_BUFFER, 0);
// 	if (bytes_read <= 0) {
// 		return "No message in connection - RECV failed";
// 	} else {
// 		buff[bytes_read] = '\0';
// 		temp += buff;
// 	}
// 	return temp;
// }
//
// bool        SocketConnection::write_connection(std::string message){
//     char* tempStr = new char[(message.size())];
// 	if (send(fd, message.c_str(), message.size(), 0) < 0) {
// 		delete tempStr;
// 		return false;
// 	}
// 	delete tempStr;
// 	return true;
// }

} /* webserv */
