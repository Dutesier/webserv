#include "socket/SocketConnection.hpp"

namespace webserv {

SocketConnection::SocketConnection(int fd, smt::shared_ptr<SocketAddress> addr)
    : Socket(fd, addr, SOCK_STREAM) {}

SocketConnection::SocketConnection() : Socket() {}

SocketConnection::~SocketConnection(void) { this->close(); }

void SocketConnection::close(void) {

    if (m_fd == -1) { return; }
    if (::close(m_fd) < 0) { throw(CloseFailureException()); }
    m_fd = -1;
}

std::string SocketConnection::recv(void) {

    char buff[READING_BUFFER + 1];
    int  bytes_read;

    bytes_read = ::recv(m_fd, &buff, READING_BUFFER, 0);
    if (bytes_read < 0) { throw(SendFailureException()); }
    if (bytes_read == 0) { return (""); }

    return (std::string(buff, bytes_read));
}

void SocketConnection::send(std::string message) {

    if (::send(m_fd, message.c_str(), message.size(), 0) < 0) {
        throw(SendFailureException());
    }
}

std::string SocketConnection::getNextRequest(std::string reqStr) {

    static std::string buff;
    std::string        ret;

    // adding reqStr to buff
    if (reqStr != "") { buff += reqStr; }

    size_t end_headers = buff.find("\r\n\r\n");
    // Request is incomplete
    if (end_headers == std::string::npos) { return (""); }

    // getting request until the end of headers
    ret = buff.substr(0, end_headers + 4);

    // getting Content-Length
    size_t pos;
    if ((pos = ret.find("Content-Length: ")) != std::string::npos) {

        // getting Content-Length
        std::string        l;
        std::istringstream iss(buff.substr(pos + 16));
        iss >> l;

        // converting to int
        int               len;
        std::stringstream ss(l);
        ss >> len;

        // adding body to ret
        if (len) { ret += buff.substr(end_headers + 4, len); }
    }

    buff = buff.substr(ret.size());

    return (ret);
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
