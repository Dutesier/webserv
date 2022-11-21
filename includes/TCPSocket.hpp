#ifndef TCP_SOCKET_HPP
#define TCP_SOCKET_HPP

#include "Socket.hpp"
#include "SocketConnection.hpp"

#include <unistd.h>
#include <vector>

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
        bool bind(void);
        bool listen(void);
        bool setsockopt(int level, int optname, const void* optval,
                        socklen_t optlen);
        // TODO: change return val
        bool accept(void);
        bool shutdown(int how);
        bool close(void);

        std::string recv(SocketConnection* connection);
        bool        send(SocketConnection* connection, std::string response);

        // TODO: refactor this to a more safe and smart approach
        std::vector<SocketConnection*> get_connections(void) const;

    private:

        /* Other Private Functions */
        std::vector<SocketConnection*> connections;
};

} // namespace webserv

#endif /* TCP_SOCKET_HPP */
