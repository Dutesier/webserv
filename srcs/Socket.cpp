#include "Socket.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

Socket::Socket(int domain, in_port_t port)
    : fd(socket(domain, SOCK_STREAM, 0)), address(this->init_address(port, domain)) {}

// Socket::~Socket( void ) {}

/* ************************************************************************** */
/* Getters and Setters                                                        */
/* ************************************************************************** */

int Socket::getFD(void) const { return (this->fd); }

/* ************************************************************************** */
/* Other Functions                                                            */
/* ************************************************************************** */

bool	Socket::bind_to_port(){
	// static_cast<struct sockaddr*>(const_cast<struct sockaddr_in*>(this->address)),
	int i = bind(this->fd,
		(struct sockaddr*)this->address,
		sizeof(address));
}

struct sockaddr_in const*	Socket::init_address(in_port_t port, int domain) {

	struct sockaddr_in const* address = new struct sockaddr_in;
	const_cast<struct sockaddr_in*>(address)->sin_family = domain;
	const_cast<struct sockaddr_in*>(address)->sin_port = htons(port);
	const_cast<struct sockaddr_in*>(address)->sin_addr.s_addr = htonl(INADDR_ANY);
	return (address);
}

/* ************************************************************************** */
/* Class Exceptions                                                           */
/* ************************************************************************** */
