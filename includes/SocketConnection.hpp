#ifndef SOCKETCONNECTION_HPP
#define SOCKETCONNECTION_HPP

#include "Logger.hpp"
#include "Socket.hpp"
#include "SocketAddress.hpp"

#include <unistd.h>
#include <vector>

#ifndef READING_BUFFER
# define READING_BUFFER 1024
#endif

namespace webserv {

class SocketConnection : public webserv::Socket {

    public:

        /* Constructors and Destructors */
        SocketConnection(int fd, SocketAddress addr);
        SocketConnection(void);
        ~SocketConnection(void);

        /* Other Functions */
        void close(void);

        std::string recv(void);
        void        send(std::string message);

        /* Exceptions */
        struct CloseFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct SendFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct RecvFailureException : public std::exception {
                char const* what(void) const throw();
        };
};

} // namespace webserv

#endif
