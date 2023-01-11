#ifndef TCP_SOCKET_HPP
#define TCP_SOCKET_HPP

#include "socket/Socket.hpp"
#include "socket/SocketConnection.hpp"
#include "utils/Logger.hpp"
#include "utils/smt.hpp"

#include <map>
#include <unistd.h>

// TODO: make this either specific to each socket or to each server
// That way we can tests specific backlogs
#ifndef BACKLOG
# define BACKLOG 10
#endif

namespace webserv {

class TCPSocket : public Socket {

    public:

        TCPSocket(int port, std::string host = "localhost",

        // TODO: understand what should be const and whatnot
        void bind(void);
        void listen(void);
        void setsockopt(int level, int optname, const void* optval,
                        socklen_t optlen);
        int  accept(void);
        void shutdown(int how);
        void close(void);

        std::string recv(int connect_fd);
        void        send(int connect_fd, std::string response);

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

        struct ShutdownFailureException : public std::exception {
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

        std::map<int, smt::shared_ptr<SocketConnection> > m_connection;
};

} // namespace webserv

#endif /* TCP_SOCKET_HPP */
