#include "HTTPServer.hpp"

#include <sys/epoll.h>

namespace webserv {

HTTPServer::HTTPServer(int argc, char* argv[])
    : state(ready), config(new Config(argc, argv)) {}

HTTPServer::~HTTPServer(void) {
    delete this->config;
    if (this->state > ready)
        for (std::vector<TCPSocket*>::iterator it = this->sockets.begin();
             it != this->sockets.end(); ++it)
            delete *it;
}

void HTTPServer::start(void) {

    struct epoll_event event;
    if ((this->epollfd = epoll_create1(0)) < 0) // creating an epoll instance
        throw(EpollCreateException());
    // initializing all TCPSockets
    std::vector<ServerConfig*> server = config->server_config();
    for (std::vector<ServerConfig*>::iterator it = server.begin();
         it != server.end(); it++) {
        init_socket(*it);
    }
    this->state = started;
}

void HTTPServer::run(void) {
    struct epoll_event ev, events[EP_MAX_EVENTS];
    this->state = running;
    while (this->state == running) {
        int nfds = epoll_wait(this->epollfd, events, EP_MAX_EVENTS, EP_TIMEOUT);
        if (nfds < 0) throw(EpollWaitException());
        for (int i = 0; i < nfds; i++) {
            if (this->has_socket(events[i].data.fd)) {
                // TCPSocket accepts request
                TCPSocket* sock = this->get_socket(events[i].data.fd);
                sock->accept();
                // A connection socket is created and inserted in epoll's
                // interest list
                int fd = sock->get_connections().back()->sockfd();
                ev.data.fd = fd;
                ev.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, fd, &ev) < 0)
                    throw(EpollAddException());
                FLOG_D("webserv::HTTPServer ACK()");
            } else {
                FLOG_D("webserv::HTTPServer REQ()");
            }
        }
    }
}

void HTTPServer::stop(void) {
    for (std::vector<TCPSocket*>::iterator it = this->sockets.begin();
         it != this->sockets.end(); ++it) {
        (*it)->close();
    }
    this->state = stoped;
}

TCPSocket* HTTPServer::get_socket(int fd) {
    for (std::vector<TCPSocket*>::iterator it = this->sockets.begin();
         it != this->sockets.end(); ++it)
        if ((*it)->sockfd() == fd) return (*it);
    return (NULL);
}

bool HTTPServer::has_socket(int fd) {
    for (std::vector<TCPSocket*>::iterator it = this->sockets.begin();
         it != this->sockets.end(); ++it)
        if ((*it)->sockfd() == fd) return (true);
    return (false);
}

void HTTPServer::init_socket(ServerConfig* server) {
    int            enable = 1;
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    // creating socket
    TCPSocket* sock = new TCPSocket(server->port, server->host);
    this->sockets.push_back(sock);
    // setting socket options
    sock->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    sock->setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    sock->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));
    // binding socket
    sock->bind();
    // socket is now in listenning mode
    sock->listen();
    // adding socket to epollfd's interest list
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = sock->sockfd();
    if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, sock->sockfd(), &event) < 0)
        throw(EpollAddException());
}

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
