#include "http/HTTPServer.hpp"

#include <sys/epoll.h>

namespace webserv {

// initializes the Config instance
HTTPServer::HTTPServer(int argc, char* argv[])
    : state(ready), config(new Config(argc, argv)) {}

HTTPServer::~HTTPServer(void) { delete this->config; }

// initializes all ServerSockets based on the config file, and adds them to an
// epoll instance's interest list
void HTTPServer::start(void) {

    struct epoll_event event;
    // creating an epoll instance
    if ((this->epollfd = epoll_create1(0)) < 0) throw(EpollCreateException());

    // initializing sockets
    std::vector<ServerConfig*> server = config->server_config();
    for (std::vector<ServerConfig*>::iterator it = server.begin();
         it != server.end(); it++) {
        ServerSocket* sock = new ServerSocket(*it);
        this->sockets[sock->sockfd()] = sock;
        epoll_add(sock->sockfd());
    }
    this->state = started;
}

// webserver starts running here. It handles two types of requests: connection
// requests for new clients and send requests for clients that are already
// connected to ServerSocket
void HTTPServer::run(void) {

    struct epoll_event events[EP_MAX_EVENTS];
    this->state = running;

    while (this->state == running) {

        int nfds = epoll_wait(this->epollfd, events, EP_MAX_EVENTS, EP_TIMEOUT);
        if (this->state != running) break;
        if (nfds < 0) throw(EpollWaitException());

        for (int i = 0; i < nfds; i++) {
            if (this->sockets.find(events[i].data.fd) != this->sockets.end()) {
                int fd = this->sockets[events[i].data.fd]->accept();
                epoll_add(fd);
                FLOG_D("webserv::HTTPServer ACK()");
            }
            else {
                std::map<int, ServerSocket*>::iterator it;
                for (it = this->sockets.begin(); it != this->sockets.end();
                     it++) {
                    if ((*it).second->has_connection(events[i].data.fd)) {
                        FLOG_D("webserv::HTTPServer REQ()");
                        http_handle((*it).second, events[i].data.fd);
                        break;
                    }
                }
            }
        }
    }
}

// stops webserver and destroys all ServerSockets
void HTTPServer::stop(void) {

    this->state = stoped;

    // deleting all sockets
    for (std::map<int, ServerSocket*>::iterator it = this->sockets.begin();
         it != this->sockets.end(); it++)
        delete (*it).second;
}

// adds a socket to epoll's interest list
void HTTPServer::epoll_add(int fd) {

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;

    if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, fd, &event) < 0)
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
