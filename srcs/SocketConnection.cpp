#include "SocketConnection.hpp"

namespace webserv {

SocketConnection::SocketConnection(int fd, SocketAddress addr)
    : Socket(fd, addr, SOCK_STREAM) {
    FLOG_D("webserv::SocketConnection created a socket");
}

SocketConnection::SocketConnection() : Socket() {
    FLOG_D("webserv::SocketConnection created a socket");
}

SocketConnection::~SocketConnection(void) {
    FLOG_D("webserv::SocketConnection destroyed a socket");
    this->close();
}

void SocketConnection::close(void) {
    if (this->fd == -1) return;
    if (::close(this->fd) < 0) throw(CloseFailureException());
    this->fd = -1;
    FLOG_D("webserv::SocketConnection closed a socket");
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

void SocketConnection::send(std::string message) {
    if (::send(fd, message.c_str(), message.size(), 0) <= 0)
        throw(SendFailureException());
}

char const* SocketConnection::CloseFailureException::what(void) const throw() {
    return ("webserv::SocketConnection failure in close()");
}

char const* SocketConnection::SendFailureException::what(void) const throw() {
    return ("webserv::SocketConnection failure in send()");
}

char const* SocketConnection::RecvFailureException::what(void) const throw() {
    return ("webserv::SocketConnection failure in recv()");
}
} // namespace webserv
