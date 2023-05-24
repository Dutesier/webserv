#ifndef SOCKET_CONNECTION_HPP
#define SOCKET_CONNECTION_HPP

#include "http/HTTPParser.hpp"
#include "socket/ConnectionAddress.hpp"
#include "utils/Logger.hpp"

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>

namespace webserv {

#define READING_BUFFER 8192

class SocketConnection {

    public:

        SocketConnection(int sockFd, sockaddr_in* addr, socklen_t addrLen);
        ~SocketConnection(void);

        int         getSockFd(void) const;
        int         getPort(void) const;
        std::string getHost(void) const;
        sa_family_t getFamily(void) const;
        sockaddr*   getAddress(void) const;
        socklen_t   getLength(void) const;

        void close(void);

        std::string recv(void);
        void        send(std::string response);

        struct CloseFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct SendFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct RecvFailureException : public std::exception {
                char const* what(void) const throw();
        };

        // Every instance of a socket connection point to a different parser
        // (stores incomplete requests)
        // TODO: this only needs to exist inside the HTTPHandler. So maybe its a
        // good idea to store it elsewhere
        smt::shared_ptr<HTTPParser> m_parser;

    private:

        std::string toString(void) const;

        smt::shared_ptr<ConnectionAddress> m_addr;
        int                                m_sockFd;
};

} // namespace webserv

#endif /* SOCKET_CONNECTION_HPP */
