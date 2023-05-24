#include "server/HTTPServer.hpp"

#include <sys/epoll.h>

namespace webserv {

// initializes the Config instance
HTTPServer::HTTPServer(int argc, char* argv[]) : m_state(ready) {

    smt::shared_ptr<Config> config(new Config(argc, argv));
    ConfigSocket::setBlocks(config->getBlocks());
}

HTTPServer::~HTTPServer(void) { close(m_epollFd); }

// initializes all ServerSockets based on the config file, and adds them to an
// epoll instance's interest list
void HTTPServer::start(void) {

    // creating an epoll instance
    if ((m_epollFd = epoll_create(EP_MAX_EVENTS)) < 0) {
        throw(EpollCreateException());
    }

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
    memset(events, 0, sizeof(events));

    while (m_state == running) {

        int nfds = epoll_wait(m_epollFd, events, EP_MAX_EVENTS, EP_TIMEOUT);

        LOG_D("webserv::HTTPServer run()");

        if (m_state != running) { break; }

        if (nfds < 0) { throw(EpollWaitException()); }

        for (int i = 0; i < nfds; i++) {

            if (m_sockets.find(events[i].data.fd) != m_sockets.end()) {

                LOG_D("webserv::HTTPServer ACK()");
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
                        LOG_D("webserv::HTTPServer REQ()");
                        http_handle(sock, connnectionIterator->second,
                                    events[i].data.fd);
                        epoll_remove(events[i].data.fd);
                        sock->close(events[i].data.fd);
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
void HTTPServer::epoll_add(int fd, int events) {

    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = events;
    event.data.fd = fd;

    if (epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &event) < 0)
        throw(EpollAddException());
}

// removes a socket from epoll's interest list
void HTTPServer::epoll_remove(int fd) {

    if (epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, NULL) < 0)
        throw(EpollRemoveException());
}

void HTTPServer::initSocket(smt::shared_ptr<ServerAddress> addr) {
    // initializing socket from address
    smt::shared_ptr<ServerSocket> sock(new ServerSocket(addr));
    sock->socket();

    // setting socket options
    int enable = 1;
    sock->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    // binding and listening
    sock->bind();
    sock->listen();

    // adding socket to epoll list
    epoll_add(sock->getSockFd(), EPOLLIN);

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

char const* HTTPServer::EpollRemoveException::what(void) const throw() {
    return ("webserv::HTTPServer epoll_ctl (remove) system call failure");
}

char const* HTTPServer::EpollWaitException::what(void) const throw() {
    return ("webserv::HTTPServer epoll_wait system call failure");
}

} // namespace webserv
