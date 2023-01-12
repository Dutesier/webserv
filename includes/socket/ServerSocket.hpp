#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "server/Blocks.hpp"
#include "socket/TCPSocket.hpp"

namespace webserv {

class ServerSocket : public TCPSocket {

    public:

        ServerSocket(smt::shared_ptr<ServerBlock> config);
        ~ServerSocket(void);

    private:

        smt::shared_ptr<ServerBlock> m_config;
};

} // namespace webserv

#endif /* SERVER_SOCKET_HPP */
