#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "Config.hpp"
#include "ServerConfig.hpp"
#include "TCPSocket.hpp"

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

        typedef enum s_state { ready, started, running, stoped } t_state;

        /* Other Functions */
        TCPSocket* get_socket(int fd);
        bool       has_socket(int fd);
        void       init_socket(ServerConfig* server);

        /* Private Attributes */
        t_state state;
        Config* config;
        int     epollfd;

        std::vector<TCPSocket*> sockets;
};

} // namespace webserv

#endif /* HTTP_SERVER_HPP */
