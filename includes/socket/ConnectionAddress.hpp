#ifndef CONNECTION_ADDRESS_HPP
#define CONNECTION_ADDRESS_HPP

#include "utils/Logger.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
#include <string>

namespace webserv {

class ConnectionAddress {

    public:

        ConnectionAddress(sockaddr_in* addr, socklen_t addrLen);
        ~ConnectionAddress(void);

        int         getPort(void) const;
        std::string getHost(void) const;
        sa_family_t getFamily(void) const;
        sockaddr*   getAddress(void) const;
        socklen_t   getLength(void) const;

    private:

        std::string toString(void) const;

        sockaddr_in* m_addr;
        socklen_t    m_len;
};

} // namespace webserv

#endif /* CONNECTION_ADDRESS_HPP */