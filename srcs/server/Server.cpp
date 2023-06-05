#include "server/Server.hpp"

#include "config/Options.hpp"
#include "http/Handler.hpp"
#include "server/Middleware.hpp"

namespace webserv {

Server::Server(void) : m_state(Ready) {}

Server::~Server(void) { close(m_epollFd); }

void Server::startServer(void) {

    // creating an epoll instance
    if ((m_epollFd = epoll_create(EP_MAX_EVENTS)) < 0) {
        throw(EpollCreateException());
    }

    // initializing sockets
    std::set< std::pair<int, std::string> > socketOpts =
        config::Options::getSocketOptions();

    std::set< std::pair<int, std::string> >::iterator it;
    for (it = socketOpts.begin(); it != socketOpts.end(); it++) {
        startSocket((*it).first, (*it).second);
    }

    // updating state
    m_state = Started;
}

void Server::runServer(void) {

    // updating state
    m_state = Running;

    struct epoll_event events[EP_MAX_EVENTS];
    memset(events, 0, sizeof(events));

    while (m_state == Running) {

        // Wait for events on registered file descriptors
        int nfds = epoll_wait(m_epollFd, events, EP_MAX_EVENTS, EP_TIMEOUT);

        if (m_state != Running) { break; }

        if (nfds < 0) { throw(EpollWaitException()); }

        for (int i = 0; i < nfds; i++) {

            if (m_sockets.find(events[i].data.fd) != m_sockets.end()) {
                // Accept new connection
                int fd = m_sockets[events[i].data.fd]->accept();
                // Add socket connection to epoll interest list
                epollAdd(fd);
            }
            else {
                // Existing connection with incoming data
                std::map<int, smt::shared_ptr<net::ServerSocket> >::iterator it;
                for (it = m_sockets.begin(); it != m_sockets.end(); it++) {

                    smt::shared_ptr<net::ServerSocket> sock = (*it).second;
                    sock->getConnection(events[i].data.fd);

                    Middleware::handleRecv(sock, events[i].data.fd);

                    // // checking if connection is done sending data
                    // try {
                    //     sock->getConnection(events[i].data.fd);
                    //     LOG_D("Socket not closed, still needs to receive
                    //     more"
                    //           "things");
                    // }
                    // catch (
                    //     net::ServerSocket::NoSuchConnectionException const&)
                    //     { epollRemove(events[i].data.fd);
                    // }
                    break;
                }
            }
        }
    }
}

void Server::stopServer(void) { m_state = Stopped; }

void Server::startSocket(int port, std::string const& host) {
    // initializing socket from address
    smt::shared_ptr<net::ServerSocket> sock;
    sock = smt::make_shared(new net::ServerSocket(port, host));

    sock->socket();

    // setting socket options
    int enable = 1;
    sock->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    sock->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));

    // binding and listening
    sock->bind();
    sock->listen();

    // adding socket to epoll list
    epollAdd(sock->getSockFd(), EPOLLIN);

    // adding socket to sockets list
    m_sockets.insert(std::make_pair(sock->getSockFd(), sock));
}

void Server::epollAdd(int fd, int events) {
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = events;
    event.data.fd = fd;

    if (epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &event) < 0) {
        throw(EpollAddException());
    }
}

void Server::epollRemove(int fd) {

    if (epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, NULL) < 0) {
        throw(EpollRemoveException());
    }
}

smt::shared_ptr<Server> Server::getInstance(void) {
    static smt::shared_ptr<Server> ist(new Server());
    return (ist);
}

void Server::start(void) { getInstance()->startServer(); }

void Server::run(void) { getInstance()->runServer(); }

void Server::stop(void) { getInstance()->stopServer(); }

char const* Server::EpollCreateException::what(void) const throw() {
    return ("webserv::Server epoll_create system call failure");
}

char const* Server::EpollAddException::what(void) const throw() {
    return ("webserv::Server epoll_ctl (add) system call failure");
}

char const* Server::EpollRemoveException::what(void) const throw() {
    return ("webserv::Server epoll_ctl (remove) system call failure");
}

char const* Server::EpollWaitException::what(void) const throw() {
    return ("webserv::Server epoll_wait system call failure");
}

} // namespace webserv
