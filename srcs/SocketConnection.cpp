#include "SocketConnection.hpp"

namespace webserv {

SocketConnection::SocketConnection(int fd, SocketAddress addr)
    : Socket(fd, addr, SOCK_STREAM) {}

SocketConnection::SocketConnection() : Socket() {}

SocketConnection::~SocketConnection(void) { this->close(); }

bool SocketConnection::close(void) {
    if (::close(this->fd) < 0) return (false);
    this->fd = -1;
    return (true);
}

// TODO: find smarter ways to get buf
std::string SocketConnection::recv(void) {
    char        buff[READING_BUFFER + 1];
    size_t      bytes_read;
    std::string temp;

    bytes_read = ::recv(fd, &buff, READING_BUFFER, 0);
    if (bytes_read <= 0) return ("");
    buff[bytes_read] = '\0';
    temp += buff;

    return (temp);
}

bool SocketConnection::send(std::string message) {
    return (::send(fd, message.c_str(), message.size(), 0) > 0);
}

} // namespace webserv
