#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "server/ServerConfig.hpp"
#include "socket/TCPSocket.hpp"

namespace webserv {

class ServerSocket : public TCPSocket {

    public:

        ServerSocket(smt::shared_ptr<ServerConfig> config);
        ~ServerSocket(void);

    private:

        smt::shared_ptr<ServerConfig> m_config;
};

} // namespace webserv

#endif /* SERVER_SOCKET_HPP */
