#include "Socket.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

Socket::Socket(int domain, in_port_t port)
    : fd(socket(domain, SOCK_STREAM, 0)) {
	this->init_address(port, domain);
}

// Socket::~Socket( void ) {}

/* ************************************************************************** */
/* Getters and Setters                                                        */
/* ************************************************************************** */

int Socket::getFD(void) const { return (this->fd); }

/* ************************************************************************** */
/* Other Functions                                                            */
/* ************************************************************************** */

bool	Socket::bind_to_port(){
	return ( bind(this->fd, (struct sockaddr*)&this->address, sizeof(address)) == 0);
}

void	Socket::init_address(in_port_t port, int domain) {
	this->address.sin_family = domain;
	this->address.sin_port = htons(port);
	this->address.sin_addr.s_addr = htonl(INADDR_ANY);
}

/* ************************************************************************** */
/* Class Exceptions                                                           */
/* ************************************************************************** */
