#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "SocketAddress.hpp"

#include <sys/socket.h>

namespace webserv {

class Socket {

    public:

        /* Constructors and Destructors */
        Socket(int fd, smt::shared_ptr<SocketAddress> addr,
               int type = SOCK_STREAM);
        Socket(int port, std::string host = "*", int family = AF_INET,
               int type = SOCK_STREAM);
        Socket(void);
        virtual ~Socket(void);

        int                            sockfd(void) const;
        int                            type(void) const;
        smt::shared_ptr<SocketAddress> address(void) const;

        virtual void close(void) = 0;

    protected:

        int                            m_fd;
        smt::shared_ptr<SocketAddress> m_addr;
        int                            m_socktype;
};

} // namespace webserv

#endif
