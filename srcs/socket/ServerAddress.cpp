#include "socket/ServerAddress.hpp"

namespace webserv {

ServerAddress::ServerAddress(std::string port, std::string host) {
    struct addrinfo hints;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = ::getaddrinfo(host.c_str(), port.c_str(), &hints, &m_info);
    if (status != 0) {
        LOG_E("Server Address " + host + ":" + port +
              ": failure in ::getaddrinfo(): " + gai_strerror(status));
        throw GetAddInfoFailureException();
    }

    m_addr = reinterpret_cast<sockaddr_in*>(m_info->ai_addr);
    m_len = m_info->ai_addrlen;

    if (!m_info || !m_addr) {
        LOG_E("Server Address " + host + ":" + port +
              ": failure in ::getaddrinfo(): null pointer returned");
        throw GetAddInfoFailureException();
    }
}

ServerAddress::~ServerAddress(void) {
    if (m_info) { ::freeaddrinfo(m_info); }
}

int ServerAddress::getPort(void) const { return (ntohs(m_addr->sin_port)); }

std::string ServerAddress::getHost(void) const {
    return (inet_ntoa(m_addr->sin_addr));
}

sa_family_t ServerAddress::getFamily(void) const {
    return (m_addr->sin_family);
}

int ServerAddress::getType(void) const { return (m_info->ai_socktype); }

sockaddr* ServerAddress::getAddress(void) const {
    return (reinterpret_cast<sockaddr*>(m_addr));
}

socklen_t ServerAddress::getLength(void) const { return (m_len); }

std::string ServerAddress::toString(void) const {

    std::ostringstream oss;
    oss << "Server Address " << getPort() << ":" << getHost();
    return (oss.str());
}

char const* ServerAddress::GetAddInfoFailureException::what(void) const
    throw() {
    return ("Server Address: failure in ::getaddrinfo()");
}

} // namespace webserv