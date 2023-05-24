#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "config/Blocks.hpp"
#include "http/HTTPRequest.hpp"
#include "socket/ServerAddress.hpp"
#include "socket/SocketConnection.hpp"
#include "utils/Logger.hpp"
#include "utils/smt.hpp"

namespace webserv {

#ifndef BACKLOG
# define BACKLOG 10
#endif

class ServerSocket {

    public:

        ServerSocket(smt::shared_ptr<ServerAddress>& addr);
        ~ServerSocket(void);

        int         getSockFd(void) const;
        int         getPort(void) const;
        std::string getHost(void) const;
        sa_family_t getFamily(void) const;
        int         getType(void) const;
        sockaddr*   getAddress(void) const;
        socklen_t   getLength(void) const;

        smt::shared_ptr<SocketConnection> getConnection(int connectFd);
        std::map< int, smt::shared_ptr<SocketConnection> > getConnections(void);

        void socket(void);
        void bind(void);
        void listen(void);
        void setsockopt(int level, int optname, void const* optval,
                        socklen_t optlen);
        int  accept(void);
        void close(void);
        void close(int connectFd);

        std::string recv(int connectFd);
        void        send(int connectFd, std::string const& response);

        struct SocketFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct BindFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct ListenFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct SetOptFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct AcceptFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct CloseFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct SendFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct RecvFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct NoSuchConnectionException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        std::string toString(void) const;

        int                            m_sockFd;
        smt::shared_ptr<ServerAddress> m_addr;

        std::map< int, smt::shared_ptr<SocketConnection> > m_connections;
};

} // namespace webserv

#endif /* SERVER_SOCKET_HPP */