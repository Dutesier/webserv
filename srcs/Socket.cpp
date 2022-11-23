#include "Socket.hpp"

namespace webserv {

Socket::Socket(int fd, SocketAddress addr, int type)
    : fd(fd), addr(addr), socktype(type) {}

Socket::Socket(int port, std::string host, int family, int type)
    : socktype(type), fd(-1), addr(SocketAddress(port, host, family)) {}

Socket::Socket() : fd(-1), socktype(SOCK_STREAM), addr(SocketAddress()) {}

Socket::~Socket() {}

int Socket::sockfd(void) const { return (this->fd); }

int Socket::type(void) const { return (this->socktype); }

SocketAddress Socket::address(void) const { return (this->addr); }

bool Socket::supportsIPv4(void) const { return (this->addr.has_ipv4()); }

bool Socket::supportsIPv6(void) const { return (this->addr.has_ipv6()); }

} // namespace webserv
