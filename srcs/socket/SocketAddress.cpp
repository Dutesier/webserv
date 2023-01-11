#include "socket/SocketAddress.hpp"

namespace webserv {

SocketAddress::SocketAddress(void)
	: m_addr(new sockaddr_in),
	  m_len(sizeof(*m_addr)) {}

SocketAddress::SocketAddress(int port, std::string host, int family)
    : m_port_nu(port), m_host_addr(host), m_addr_family(family),
	  m_addr(new sockaddr_in) {

    m_addr->sin_family = m_addr_family;
    m_addr->sin_port = htons(m_port_nu);
    m_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    m_len = sizeof(*m_addr);
}

SocketAddress::SocketAddress(SocketAddress const& src) : m_addr(NULL) {
    *this = src;
}

SocketAddress::~SocketAddress(void) {
	delete m_addr;
}

SocketAddress& SocketAddress::operator=(SocketAddress const& rhs) {

    m_host_addr = rhs.m_host_addr;
    m_addr_family = rhs.m_addr_family;
    m_port_nu = rhs.m_port_nu;

    m_addr = new sockaddr_in;

    m_addr->sin_port = htons(m_port_nu);
    m_addr->sin_family = m_addr_family;
    m_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    m_len = sizeof(*m_addr);

    return (*this);
}

struct sockaddr* SocketAddress::address(void) const {
    return (reinterpret_cast<sockaddr*>(m_addr));
}

socklen_t SocketAddress::length(void) const { return (m_len); }

socklen_t* SocketAddress::length_ptr(void) { return (&m_len); }

int SocketAddress::port(void) const { return (m_port_nu); }

std::string SocketAddress::host(void) const { return (m_host_addr); }

int SocketAddress::family(void) const { return (m_addr_family); }

} // namespace webserv
