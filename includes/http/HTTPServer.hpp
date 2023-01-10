#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "http/HTTPHandler.hpp"
#include "server/Config.hpp"
#include "server/ServerConfig.hpp"
#include "socket/ServerSocket.hpp"

#include <map>
#include <sys/epoll.h>

#define EP_MAX_EVENTS 200
#define EP_TIMEOUT    3000

namespace webserv {

class HTTPServer {

    public:

        /* Constructors and Destructors */
        HTTPServer(int argc, char* argv[]);
        ~HTTPServer(void);

        /* Other Functions */
        void start(void);
        void run(void);
        void stop(void);

        /* Other Functions */
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

        /* Private Member Types */
        typedef enum s_state { ready, started, running, stoped } t_state;

        /* Other Functions */
        void epoll_add(int fd);

        /* Private Attributes */
        Config*                      config;
        std::map<int, ServerSocket*> sockets;

        t_state state;
        int     epollfd;
};

} // namespace webserv

#endif /* HTTP_SERVER_HPP */
