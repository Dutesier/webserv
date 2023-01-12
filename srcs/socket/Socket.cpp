#include "socket/Socket.hpp"

namespace webserv {

Socket::Socket(int fd, SocketAddress addr, int type)
    : m_fd(fd), m_addr(addr), m_socktype(type) {}

Socket::Socket(int port, std::string host, int family, int type)
    : m_socktype(type), m_fd(-1), m_addr(SocketAddress(port, host, family)) {}

Socket::Socket() : m_fd(-1), m_socktype(SOCK_STREAM), m_addr(SocketAddress()) {}

Socket::~Socket() {}

int Socket::sockfd(void) const { return (m_fd); }

int Socket::type(void) const { return (m_socktype); }

SocketAddress Socket::address(void) const { return (m_addr); }

} // namespace webserv
