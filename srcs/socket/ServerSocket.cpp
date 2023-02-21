#include "socket/ServerSocket.hpp"
#include <cstring>


namespace webserv {

ServerSocket::ServerSocket(std::vector< smt::shared_ptr<ServerBlock> > blocks)
    : TCPSocket((*blocks.begin())->m_port, (*blocks.begin())->m_host),
      m_blocks(blocks) {

    // setting socket options
    int            enable = 1;
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));

    // binding socket
    bind();

    // make socket start listening to requests
    listen();

    // debugging message
    std::stringstream ss;
    ss << m_addr->port();
    LOG_D("created server socket | " + m_addr->host() + ":" + ss.str());
}

ServerSocket::~ServerSocket(void) {}

int ServerSocket::bestServerBlockForRequest(smt::shared_ptr<HTTPRequest>& request){
    std::string uri = request->getRefinedResource();
    std::string ipAndPort = "";
    std::string serverName = "";
    int ret = -1;

    int i1, i2, i3, i4, p1;
    char* s1, s2;

    // Check if resource is of type 127.0.0.0:8080
    int matched = sscanf(uri.c_str(), "%d.%d.%d.%d:%d", &i1, &i2, &i3, &i4, &p1);
    if (matched != 5) {
        // Check if resource is of type 127.0.0.0
        matched = sscanf(uri.c_str(), "%d.%d.%d.%d", &i1, &i2, &i3, &i4);
        if (matched != 4) {
            // Check if resource is of type something.com
            matched = sscanf(uri.c_str(), "%s.%s", &s1, &s2);
            if (matched == 2) {
              serverName = std::string(s1) + "." + std::string(s2);
            }
        }
        else {
            ipAndPort = std::to_string(i1) + "." + std::to_string(i2) + "." + std::to_string(i3) + "." + std::to_string(i4) + ":8080";
        }
    } else {
        ipAndPort = std::to_string(i1) + "." + std::to_string(i2) + "." + std::to_string(i3) + "." + std::to_string(i4) + ":" + std::to_string(p1);
    }

    if (!ipAndPort.empty()) {
        ret = bestServerBlockByIPAndPort(ipAndPort);
    }

    if (ret == -1 && !serverName.empty()) {
        ret = bestServerBlockByServerName(serverName);
    }

    return (ret);
}

int ServerSocket::bestServerBlockByIPAndPort(std::string& ipAndPort) {
    char* tempIP;
    char* tempPort;
    sscanf(ipAndPort.c_str(), "%s:%s", &tempIP, &tempPort);

    std::string IP(tempIP);
    std::string port(tempPort);
    int index = 0;

    for (std::vector<smt::shared_ptr<ServerBlock>>::iterator it = m_blocks.begin();
        it != m_blocks.end(); it++) {
        if ((*it)->m_port == atoi(port.c_str()) && ((*it)->m_host == IP || (*it)->m_host == "*")) {
            return index;
        }
        index++;
    }
    return -1;
}

int ServerSocket::bestServerBlockByServerName(std::string& serverName) {
    int index = 0;

    for (std::vector<smt::shared_ptr<ServerBlock>>::iterator it = m_blocks.begin();
        it != m_blocks.end(); it++) {
        if ((*it)->m_server_name == serverName) {
            return index;
        }
        index++;
    }
    return -1;
}  


} // namespace webserv
