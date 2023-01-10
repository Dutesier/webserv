#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "server/ServerConfig.hpp"
#include "socket/TCPSocket.hpp"

namespace webserv {

class ServerSocket : public TCPSocket {

    public:

        /* Constructor and Destructor */
        ServerSocket(ServerConfig* config);
        ~ServerSocket(void);

    private:

        /* Private Attributes */
        ServerConfig* configs;
};

} // namespace webserv

#endif /* SERVER_SOCKET_HPP */
