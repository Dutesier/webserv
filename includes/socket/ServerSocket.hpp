#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "config/Blocks.hpp"
#include "http/HTTPRequest.hpp"
#include "socket/TCPSocket.hpp"

namespace webserv {

class ServerSocket : public TCPSocket {

    public:

        ServerSocket(std::vector< smt::shared_ptr<ServerBlock> > blocks);
        ServerSocket(int port, std::string host = "*");
        ~ServerSocket(void);

        std::string getNextRequest(int connectionFd, std::string reqStr = "");
        smt::shared_ptr<ServerBlock>
            getConfigFromRequest(smt::shared_ptr<HTTPRequest>& req);

        // int bestServerBlockForRequest(smt::shared_ptr<HTTPRequest>& request);
        std::vector< smt::shared_ptr<ServerBlock> > m_blocks;

    private:

        // int         bestServerBlockByIPAndPort(std::string& ipAndPort);
        // int         bestServerBlockByServerName(std::string& serverName);
        // std::string extractResource(std::string uri);
        // bool        startsWithServerName(std::string const& str);
        // bool        startsWithIP(std::string const& str);
        // bool        startsWithIpAndPort(std::string const& str);
};

// Parse HTTP Request -> look for URI -> check URI against listen or port server
// name from server blocks

} // namespace webserv

#endif /* SERVER_SOCKET_HPP */
