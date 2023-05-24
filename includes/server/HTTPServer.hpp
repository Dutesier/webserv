#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "config/Blocks.hpp"
#include "config/Config.hpp"
#include "config/ConfigSocket.hpp"
#include "http/HTTPHandler.hpp"
#include "socket/ServerSocket.hpp"

#include <map>
#include <sys/epoll.h>

#define EP_MAX_EVENTS 200
#define EP_TIMEOUT    3000

namespace webserv {

class HTTPServer {

    private:

        typedef enum s_state { ready, started, running, stopped } t_state;

    public:

        HTTPServer(int argc, char* argv[]);
        ~HTTPServer(void);

        void start(void);
        void run(void);
        void stop(void);

        struct EpollCreateException : public std::exception {
                char const* what(void) const throw();
        };

        struct EpollAddException : public std::exception {
                char const* what(void) const throw();
        };

        struct EpollWaitException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        void epoll_add(int fd);

        void initSocket(smt::shared_ptr<ServerAddress> addr);

        t_state                                       m_state;
        std::map<int, smt::shared_ptr<ServerSocket> > m_sockets;
        int                                           m_epollFd;
};

} // namespace webserv

#endif /* HTTP_SERVER_HPP */
