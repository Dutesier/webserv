#ifndef SERVER_ADDRESS_HPP
#define SERVER_ADDRESS_HPP

#include "utils/Logger.hpp"
#include "utils/smt.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <sstream>
#include <sys/socket.h>

namespace webserv {

class ServerAddress {

    public:

        ServerAddress(std::string port, std::string host);
        ~ServerAddress(void);

        int         getPort(void) const;
        std::string getHost(void) const;
        sa_family_t getFamily(void) const;
        int         getType(void) const;
        sockaddr*   getAddress(void) const;
        socklen_t   getLength(void) const;
        socklen_t*  getPtrLength(void);

        struct GetAddInfoFailureException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        std::string toString(void) const;

        addrinfo*    m_info;
        sockaddr_in* m_addr;
        socklen_t    m_len;
};

} // namespace webserv

#endif /* SERVER_ADDRESS_HPP */