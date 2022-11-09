#include "SocketAddress.hpp"

namespace webserv {

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

SocketAddress::SocketAddress(int port, std::string host, int family)
	: port_nu(port), host_addr(host), addr_family(family) {

	this->addr = new struct sockaddr_in;
	this->addr->sin_family = this->addr_family;
	this->addr->sin_port = htons(this->port_nu);
	this->addr->sin_addr.s_addr = htonl(INADDR_ANY);
	this->len = sizeof(*this->addr);
}

SocketAddress::~SocketAddress( void ) { delete this->addr; }

/* ************************************************************************** */
/* Getters and Setters                                                        */
/* ************************************************************************** */

struct sockaddr*	SocketAddress::address(void) const {
	return (reinterpret_cast<struct sockaddr*>(this->addr));
}

socklen_t	SocketAddress::length(void) const { return (this->len); }

int	SocketAddress::port(void) const { return (this->port_nu); }

std::string	SocketAddress::host(void) const { return (this->host_addr); }

int	SocketAddress::family(void) const { return (this->addr_family); }

bool	SocketAddress::has_ipv4(void) const {
	return (this->addr_family == AF_INET || this->addr_family == AF_INET6);
}

bool	SocketAddress::has_ipv6(void) const {
	return (this->addr_family == AF_INET6);
}

}	/* webserv */
