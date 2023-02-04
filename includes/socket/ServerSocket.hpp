#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "config/Blocks.hpp"
#include "socket/TCPSocket.hpp"

namespace webserv {

class ServerSocket : public TCPSocket {

    public:

        ServerSocket(std::vector< smt::shared_ptr<ServerBlock> > blocks);
        ~ServerSocket(void);

        std::vector< smt::shared_ptr<ServerBlock> > m_blocks;
};

} // namespace webserv

#endif /* SERVER_SOCKET_HPP */
