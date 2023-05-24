#include "server/HTTPServer.hpp"

#include <sys/epoll.h>

namespace webserv {

// initializes the Config instance
HTTPServer::HTTPServer(int argc, char* argv[])
    : m_state(ready),
      m_config(smt::shared_ptr<Config>(new Config(argc, argv))) {
    // this will create a config instance and will set the blocks to a macro
    // class This macro class will have all the necessary configurations
}

HTTPServer::~HTTPServer(void) {}

// initializes all ServerSockets based on the config file, and adds them to an
// epoll instance's interest list
void HTTPServer::start(void) {

    // creating an epoll instance
    if ((m_epollfd = epoll_create1(0)) < 0) { throw(EpollCreateException()); }

    // initializing sockets
    ConfigSocket socket_init(m_config->getBlocks());

    std::set< std::pair<unsigned, std::string> > specs = socket_init.specs();
    std::set< std::pair<unsigned, std::string> >::iterator it;
    for (it = specs.begin(); it != specs.end(); it++) {

        // getting config block associated to socket
        std::vector< smt::shared_ptr<ServerBlock> > block =
            socket_init.blocks(*it);

        // creating socket
        smt::shared_ptr<ServerSocket> sock(new ServerSocket(block));
        m_socket[sock->sockfd()] = sock;

        // adding socket to epoll list
        epoll_add(sock->sockfd());
    }

    // updating state
    m_state = started;
}

// webserver starts running here. It handles two types of requests: connection
// requests for new clients and send requests for clients that are already
// connected to ServerSocket
void HTTPServer::run(void) {

    // updating state
    m_state = running;

    struct epoll_event events[EP_MAX_EVENTS];

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
                    std::map<int, smt::shared_ptr<webserv::SocketConnection> >::
                        iterator connnectionIterator;
                    connnectionIterator =
                        sock->m_connection.find(events[i].data.fd);
                    if (connnectionIterator != sock->m_connection.end()) {
                        FLOG_D("webserv::HTTPServer REQ()");
                        http_handle(sock, connnectionIterator->second,
                                    events[i].data.fd);
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
