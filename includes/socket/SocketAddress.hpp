#ifndef SOCKET_ADDRESS_HPP
#define SOCKET_ADDRESS_HPP

#include "utils/smt.hpp"

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

namespace webserv {

class SocketAddress {

    public:

        /* Constructors and Destructors */
        SocketAddress(void);
        SocketAddress(int port, std::string host = "localhost",
                      int family = AF_INET);
        SocketAddress(SocketAddress const& src);

        ~SocketAddress(void);

        /* Operator's Overload */
        SocketAddress& operator=(SocketAddress const& rhs);

        /* Getters and Setters */
        struct sockaddr* address(void) const;

        socklen_t   length(void) const;
        socklen_t*  length_ptr(void);
        std::string host(void) const;

        int port(void) const;
        int family(void) const;

    private:

        /* Private Attributes */
		sockaddr_in* m_addr;

        socklen_t   m_len;
        std::string m_host_addr;

        int m_port_nu;
        int m_addr_family;
};

} // namespace webserv

#endif /* SOCKET_ADDRESS_HPP */
