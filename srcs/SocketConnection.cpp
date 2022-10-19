#include "SocketConnection.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

SocketConnection::SocketConnection(){
    if (this->address != NULL)
        this->address = new struct sockaddr;
}

SocketConnection::~SocketConnection(){
    if (this->fd != -1)
		close(this->fd);
	if (this->address)
		delete(this->address);
}


/* ************************************************************************** */
/* Setters and Getters                                                        */
/* ************************************************************************** */

void SocketConnection::set_fd(int client_connection){
    this->fd = client_connection;
}

void SocketConnection::set_address(struct sockaddr* addr){
    if (this->address != NULL)
		delete this->address;
	this->address = addr;
}

socklen_t* SocketConnection::get_addr_len_ptr(void) {
    return &(this->addr_len);
}


/* ************************************************************************** */
/* I/O handling                                                               */
/* ************************************************************************** */
std::string SocketConnection::read_connection(void) {
	char buff[READING_BUFFER + 1]; // This will probably not be a fized size
	size_t bytes_read;
	std::string temp;
	//int maximum_iterations = 65535 / READING_BUFFER; // This is equivalent to having a 64Kb packet (TCP max packet size)

	// while ((bytes_read = recv(fd, &buff, READING_BUFFER, 0)) > 0) {
	// 	if (!maximum_iterations)
	// 		break;
	// 	buff[bytes_read] = '\0';
	// 	temp += buff;
	// 	--maximum_iterations;
	// 	if (bytes_read < READING_BUFFER)
	// 		break;
	// }
	// if (maximum_iterations == 65535 / READING_BUFFER) // No loop was done
		// return "No message in connection - RECV failed";

	bytes_read = recv(fd, &buff, READING_BUFFER, 0);
	if (bytes_read <= 0) {
		return "No message in connection - RECV failed";
	} else {
		buff[bytes_read] = '\0';
		temp += buff;
	}
	return temp;
}

bool        SocketConnection::write_connection(std::string message){
    char* tempStr = new char[(message.size())];
	if (send(fd, message.c_str(), message.size(), 0) < 0) {
		delete tempStr;
		return false;
	}
	delete tempStr;
	return true;
}
