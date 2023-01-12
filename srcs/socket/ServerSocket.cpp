#include "socket/ServerSocket.hpp"

namespace webserv {

ServerSocket::ServerSocket(smt::shared_ptr<ServerBlock> config)
    : TCPSocket(config->m_port, config->m_host) {

    int            enable = 1;
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    // setting socket options
    setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));
    // binding socket
    bind();
    // socket is now in listenning mode
    listen();
}

ServerSocket::~ServerSocket(void) {}

} // namespace webserv
