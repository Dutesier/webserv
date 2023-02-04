#include "socket/ServerSocket.hpp"

namespace webserv {

ServerSocket::ServerSocket(std::vector< smt::shared_ptr<ServerBlock> > blocks)
    : TCPSocket((*blocks.begin())->m_port, (*blocks.begin())->m_host),
      m_blocks(blocks) {

    // setting socket options
    int            enable = 1;
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));

    // binding socket
    bind();

    // make socket start listening to requests
    listen();

	// debugging message
	std::stringstream ss;
	ss << m_addr.port();
	LOG_D("created server socket | " + m_addr.host() + ":" + ss.str());
}

ServerSocket::~ServerSocket(void) {}

} // namespace webserv
