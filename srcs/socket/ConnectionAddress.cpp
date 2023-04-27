#include "socket/ConnectionAddress.hpp"

namespace webserv {

ConnectionAddress::ConnectionAddress(sockaddr_in* addr, socklen_t addrLen)
    : m_addr(addr), m_len(addrLen) {}

ConnectionAddress::~ConnectionAddress(void) { delete m_addr; }

int ConnectionAddress::getPort(void) const { return (ntohs(m_addr->sin_port)); }

std::string ConnectionAddress::getHost(void) const {
    return (inet_ntoa(m_addr->sin_addr));
}

sa_family_t ConnectionAddress::getFamily(void) const {
    return (getAddress()->sa_family);
}

sockaddr* ConnectionAddress::getAddress(void) const {
    return (reinterpret_cast<sockaddr*>(m_addr));
}

socklen_t ConnectionAddress::getLength(void) const { return (m_len); }

std::string ConnectionAddress::toString(void) const {

    std::ostringstream oss;
    oss << "Connection Address " << getPort() << ":" << getHost();
    return (oss.str());
}

} // namespace webserv