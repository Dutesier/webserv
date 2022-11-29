#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "SocketAddress.hpp"

#include <sys/socket.h>

namespace webserv {

class Socket {

    public:

        /* Constructors and Destructors */
        Socket(int fd, SocketAddress addr, int type = SOCK_STREAM);
        Socket(int port, std::string host = "localhost", int family = AF_INET,
               int type = SOCK_STREAM);
        Socket(void);
        virtual ~Socket(void);

        /* Getters and Setters */
        int sockfd(void) const;
        int type(void) const;

        SocketAddress address(void) const;
        bool          supportsIPv4(void) const;
        bool          supportsIPv6(void) const;

        /* Other Functions */
        virtual void close(void) = 0;

    protected:

        /* Protected Attributes */
        SocketAddress addr;
        int           fd;
        int           socktype;
};

} // namespace webserv

#endif
