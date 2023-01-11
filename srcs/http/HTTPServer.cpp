#include "http/HTTPServer.hpp"

#include <sys/epoll.h>

namespace webserv {

// initializes the Config instance
HTTPServer::HTTPServer(int argc, char* argv[])
    : m_state(ready), m_config(smt::shared_ptr<Config>(new Config(argc, argv))) {}

HTTPServer::~HTTPServer(void) {}

// initializes all ServerSockets based on the config file, and adds them to an
// epoll instance's interest list
void HTTPServer::start(void) {

    struct epoll_event event;

    // creating an epoll instance
    if ((m_epollfd = epoll_create1(0)) < 0) { throw(EpollCreateException()); }

    // initializing sockets
    std::vector< smt::shared_ptr<ServerConfig> > server = m_config->server_config();

    for (std::vector< smt::shared_ptr<ServerConfig> >::iterator it = server.begin(); it != server.end(); it++) {

		smt::shared_ptr<ServerSocket> sock(new ServerSocket(*it));
        m_socket[sock->sockfd()] = sock;
        epoll_add(sock->sockfd());
    }
    m_state = started;
}

// webserver starts running here. It handles two types of requests: connection
// requests for new clients and send requests for clients that are already
// connected to ServerSocket
void HTTPServer::run(void) {

    struct epoll_event events[EP_MAX_EVENTS];
    m_state = running;

    while (m_state == running) {

        int nfds = epoll_wait(m_epollfd, events, EP_MAX_EVENTS, EP_TIMEOUT);

        if (m_state != running) { break; }
        if (nfds < 0) { throw(EpollWaitException()); }

        for (int i = 0; i < nfds; i++) {

            if (m_socket.find(events[i].data.fd) != m_socket.end()) {

                FLOG_D("webserv::HTTPServer ACK()");
                int fd = m_socket[events[i].data.fd]->accept();
                epoll_add(fd);
            }
			else {

                std::map<int, smt::shared_ptr<ServerSocket> >::iterator it;
                for (it = m_socket.begin(); it != m_socket.end(); it++) {

					smt::shared_ptr<ServerSocket> sock = (*it).second;
                    if (sock->m_connection.find(events[i].data.fd) != sock->m_connection.end()) {

                        FLOG_D("webserv::HTTPServer REQ()");
                        http_handle(sock, events[i].data.fd);
                        break;
                    }
                }
            }
        }
    }
}

// stops webserver and destroys all ServerSockets
void HTTPServer::stop(void) { m_state = stopped; }

// adds a socket to epoll's interest list
void HTTPServer::epoll_add(int fd) {

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;

    if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &event) < 0)
        throw(EpollAddException());
}

// Exceptions
char const* HTTPServer::EpollCreateException::what(void) const throw() {
    return ("webserv::HTTPServer epoll_create system call failure");
}

char const* HTTPServer::EpollAddException::what(void) const throw() {
    return ("webserv::HTTPServer epoll_ctl (add) system call failure");
}

char const* HTTPServer::EpollWaitException::what(void) const throw() {
    return ("webserv::HTTPServer epoll_wait system call failure");
}

} // namespace webserv
