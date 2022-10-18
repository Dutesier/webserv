#include "SocketConnection.hpp"

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

void SocketConnection::setFD(int client_connection){
    this->fd = client_connection;
}

socklen_t* SocketConnection::getAddrLenPointer(void) {
    return &(this->addr_len);
}