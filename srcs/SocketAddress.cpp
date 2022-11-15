#include "SocketAddress.hpp"

namespace webserv {

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

SocketAddress::SocketAddress( void ) : addr(new struct sockaddr_in) {
	this->len = sizeof(*this->addr);
}

SocketAddress::SocketAddress(int port, std::string host, int family)
	: port_nu(port), host_addr(host), addr_family(family),
	addr(new struct sockaddr_in) {

	this->addr->sin_family = this->addr_family;
	this->addr->sin_port = htons(this->port_nu);
	this->addr->sin_addr.s_addr = htonl(INADDR_ANY);
	this->len = sizeof(*this->addr);
}

SocketAddress::SocketAddress(SocketAddress const& src)
	: addr(nullptr) { *this = src; }

SocketAddress::~SocketAddress( void ) { delete this->addr; }

/* ************************************************************************** */
/* Operator's Overload                                                        */
/* ************************************************************************** */

SocketAddress&	SocketAddress::operator=(SocketAddress const& rhs) {

	this->host_addr = rhs.host_addr;
	this->addr_family = rhs.addr_family;
	this->port_nu = rhs.port_nu;
	this->ipv4 = rhs.ipv4;
	this->ipv6 = rhs.ipv6;

	if (this->addr)
		delete this->addr;
	this->addr = new struct sockaddr_in;

	this->addr->sin_port = htons(this->port_nu);
	this->addr->sin_family = this->addr_family;
	this->addr->sin_addr.s_addr = htonl(INADDR_ANY);
	this->len = sizeof(*this->addr);
	return (*this);
}

/* ************************************************************************** */
/* Getters and Setters                                                        */
/* ************************************************************************** */

struct sockaddr*	SocketAddress::address(void) const {
	return (reinterpret_cast<struct sockaddr*>(this->addr));
}

socklen_t	SocketAddress::length(void) const { return (this->len); }

socklen_t*	SocketAddress::length_ptr(void) { return ( &this->len ); }

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
