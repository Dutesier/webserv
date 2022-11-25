#ifndef TCP_SOCKET_HPP
#define TCP_SOCKET_HPP

#include "Logger.hpp"
#include "Socket.hpp"
#include "SocketConnection.hpp"
#include "smt.hpp"

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

        /* Constructors and Destructors */
        // TCPSocket(int domain, in_port_t port);
        TCPSocket(int port, std::string host = "localhost",
                  int family = AF_INET);
        ~TCPSocket(void);

        /* Other Functions */
        // TODO: understand what should be const and whatnot
        void bind(void);
        void listen(void);
        void setsockopt(int level, int optname, const void* optval,
                        socklen_t optlen);
        // TODO: change return val
        int  accept(void);
        void shutdown(int how);
        void close(void);

<<<<<<< HEAD
        std::string recv(SocketConnection* connection);
        void        send(SocketConnection* connection, std::string response);
=======
        smt::shared_ptr<HTTPRequest> recv(SocketConnection* connection);
        bool        send(SocketConnection* connection, std::string response);
>>>>>>> Dont hate me please

        // TODO: refactor this to a more safe and smart approach
        SocketConnection* connection(int fd) const;
        bool              has_connection(int fd) const;

        /* Exceptions */
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

    protected:

        /* Other Private Functions */
        std::map<int, SocketConnection*> connections;
};

} // namespace webserv

#endif /* TCP_SOCKET_HPP */
