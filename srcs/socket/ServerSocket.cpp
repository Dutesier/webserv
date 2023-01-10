#include "socket/ServerSocket.hpp"

namespace webserv {

ServerSocket::ServerSocket(ServerConfig* config)
    : TCPSocket(config->port, config->host) {

    int            enable = 1;
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    // setting socket options
    this->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    this->setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    this->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));
    // binding socket
    this->bind();
    // socket is now in listenning mode
    this->listen();
}

ServerSocket::~ServerSocket(void) {}

} // namespace webserv
