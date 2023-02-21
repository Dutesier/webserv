#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "config/Blocks.hpp"
#include "socket/TCPSocket.hpp"
#include "http/HTTPRequest.hpp"

namespace webserv {

class ServerSocket : public TCPSocket {

    public:

        ServerSocket(std::vector< smt::shared_ptr<ServerBlock> > blocks);
        ~ServerSocket(void);

        int bestServerBlockForRequest(smt::shared_ptr<HTTPRequest>& request);
        std::vector< smt::shared_ptr<ServerBlock> > m_blocks;
    
    private:
        int bestServerBlockByIPAndPort(std::string& ipAndPort);
        int bestServerBlockByServerName(std::string& serverName);

};

// PArse HTTP Request -> look for URI -> check URI against listen or port server name from server blocks

} // namespace webserv

#endif /* SERVER_SOCKET_HPP */
