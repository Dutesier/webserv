#include "server/HTTPServer.hpp"

#include <sys/epoll.h>

namespace webserv {

// initializes the Config instance and adds the blocks to ConfigSocket
HTTPServer::HTTPServer(int argc, char* argv[]) : m_state(ready) {

    smt::shared_ptr<Config> config(new Config(argc, argv));
    ConfigSocket::setBlocks(config->getBlocks());
}

HTTPServer::~HTTPServer(void) {}

// initializes all ServerSockets based on the config file, and adds them to an
// epoll instance's interest list
void HTTPServer::start(void) {

    // creating an epoll instance
    if ((m_epollfd = epoll_create1(0)) < 0) { throw(EpollCreateException()); }

    // initializing sockets
    std::set< std::pair<int, std::string> > specs = ConfigSocket::getSpecs();
    std::set< std::pair<int, std::string> >::iterator it;
    for (it = specs.begin(); it != specs.end(); it++) {
        smt::shared_ptr<ServerAddress> addr = ConfigSocket::getAddress(*it);
        initSocket(addr);
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

            if (m_sockets.find(events[i].data.fd) != m_sockets.end()) {

                FLOG_D("webserv::HTTPServer ACK()");
                int fd = m_sockets[events[i].data.fd]->accept();
                epoll_add(fd);
            }
            else {

                std::map<int, smt::shared_ptr<ServerSocket> >::iterator it;
                for (it = m_sockets.begin(); it != m_sockets.end(); it++) {

                    smt::shared_ptr<ServerSocket> sock = (*it).second;
                    std::map<int, smt::shared_ptr<webserv::SocketConnection> >
                        connections;
                    std::map<int, smt::shared_ptr<webserv::SocketConnection> >::
                        iterator connnectionIterator;
                    connections = sock->getConnections();
                    connnectionIterator = connections.find(events[i].data.fd);
                    if (connnectionIterator != connections.end()) {
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

void HTTPServer::initSocket(smt::shared_ptr<ServerAddress> addr) {
    // initializing socket from address
    smt::shared_ptr<ServerSocket> sock(new ServerSocket(addr));
    sock->socket();

    // setting socket options
    int            enable = 1;
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    sock->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    sock->setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    sock->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));

    // binding and listening
    sock->bind();
    sock->listen();

    // adding socket to epoll list
    epoll_add(sock->getSockFd());

    // adding socket to sockets list
    m_sockets[sock->getSockFd()] = sock;
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
