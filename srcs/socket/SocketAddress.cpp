#include "socket/SocketAddress.hpp"

namespace webserv {

SocketAddress::SocketAddress(void)
    : m_addr(new sockaddr_in), m_len(sizeof(*m_addr)) {}

SocketAddress::SocketAddress(int port, std::string host, int family)
    : m_addr(new sockaddr_in), m_host(host), m_port(port), m_family(family) {

    m_addr->sin_family = m_family;
    m_addr->sin_port = htons(m_port);

    unsigned long ip;
    if (m_host == "*") { ip = INADDR_ANY; }
    else { ip = inet_addr(m_host.c_str()); }

    if (ip == INADDR_NONE) {
        delete m_addr;
        throw(InvalidIPAddressException());
    }

    // FIXME: for now this seems to work, but I'm really not sure if it's
    // the best way to handle this
    m_addr->sin_addr.s_addr = ip;

    m_len = sizeof(*m_addr);
}

SocketAddress::~SocketAddress(void) { delete m_addr; }

struct sockaddr* SocketAddress::address(void) const {
    return (reinterpret_cast<sockaddr*>(m_addr));
}

socklen_t SocketAddress::length(void) const { return (m_len); }

socklen_t* SocketAddress::length_ptr(void) { return (&m_len); }

int SocketAddress::port(void) const { return (m_port); }

std::string SocketAddress::host(void) const { return (m_host); }

int SocketAddress::family(void) const { return (m_family); }

char const* SocketAddress::InvalidIPAddressException::what(void) const throw() {
	return ("SocketAddress: Invalid IP address");
}

} // namespace webserv
