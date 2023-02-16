#ifndef SOCKET_ADDRESS_HPP
#define SOCKET_ADDRESS_HPP

#include "utils/smt.hpp"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

namespace webserv {

class SocketAddress {

    public:

        SocketAddress(void);
        SocketAddress(int port, std::string host = "*", int family = AF_INET);
        ~SocketAddress(void);

        sockaddr*   address(void) const;
        socklen_t   length(void) const;
        socklen_t*  length_ptr(void);
        std::string host(void) const;
        int         port(void) const;
        int         family(void) const;

        struct InvalidIPAddressException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        sockaddr_in* m_addr;
        socklen_t    m_len;
        std::string  m_host;
        int          m_port;
        int          m_family;
};

} // namespace webserv

#endif /* SOCKET_ADDRESS_HPP */
