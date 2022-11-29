#include "HTTPServer.hpp"

#include <sys/epoll.h>

namespace webserv {

HTTPServer::HTTPServer(int argc, char* argv[])
    : state(ready), config(new Config(argc, argv)) {}

HTTPServer::~HTTPServer(void) {
    delete this->config;
    if (this->state > ready) {
        for (std::map<int, TCPSocket*>::iterator it = this->sockets.begin();
             it != this->sockets.end(); it++) {
            delete (*it).second;
		}
	}
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
		if (this->state != running) break ;
        if (nfds < 0) throw(EpollWaitException());
        for (int i = 0; i < nfds; i++) {
            if (this->sockets.find(events[i].data.fd) != this->sockets.end()) {
                // TCPSocket accepts request
                TCPSocket* sock = this->sockets[events[i].data.fd];
                // A connection socket is created and inserted in epoll's
                // interest list
                int fd = sock->accept();
                ev.data.fd = fd;
                ev.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, fd, &ev) < 0)
                    throw(EpollAddException());
                FLOG_D("webserv::HTTPServer ACK()");
            } else {
                // find client socket
                SocketConnection* con = NULL;
                for (std::map<int, TCPSocket*>::iterator it =
                         this->sockets.begin();
                     it != this->sockets.end(); it++) {
                    try {
                        con = (*it).second->connection(events[i].data.fd);
                    } catch (TCPSocket::NoSuchConnectionException& e) { ; }
                    if (con) break;
                }
                FLOG_D("webserv::HTTPServer REQ()");
                std::string str = con->recv();
                std::cout << str << std::endl;
                con->send(str);
            }
        }
    }
}

void HTTPServer::stop(void) {
    this->state = stoped;
    for (std::map<int, TCPSocket*>::iterator it = this->sockets.begin();
         it != this->sockets.end(); ++it) {
        (*it).second->close();
    }
}

void HTTPServer::init_socket(ServerConfig* server) {
    int            enable = 1;
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    // creating socket
    TCPSocket* sock = new TCPSocket(server->port, server->host);
    this->sockets.insert(std::pair<int, TCPSocket*>(sock->sockfd(), sock));
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
