#include "socket/ServerSocket.hpp"

#include <cstring>

// #include <regex>

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

ServerSocket::ServerSocket(int port, std::string host) : TCPSocket(port, host) {

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

std::string ServerSocket::getNextRequest(int         connection_fd,
                                         std::string req_str) {
    typedef std::map< int, smt::shared_ptr<SocketConnection> >::iterator
        iterator;

    iterator it = m_connection.find(connection_fd);
    if (it == m_connection.end()) { throw(NoSuchConnectionException()); }

    return ((*it).second->getNextRequest(req_str));
}

// Chooses the right server block from the config file for the request type
// Need to figure out curl --resolve and host
smt::shared_ptr<ServerBlock>
    ServerSocket::getConfigFromRequest(smt::shared_ptr<HTTPRequest>& req) {

    std::string host_header = req->getHeader("Host");

    // Getting port and address
    size_t      n = host_header.find(":");
    std::string p, a;
    unsigned    port;

    if (n != std::string::npos) {
        a = host_header.substr(0, n);
        if (a == "localhost") { a = "127.0.0.1"; }
        p = host_header.substr(n + 1);
    }
    else { p = host_header; }
    if (!p.empty()) {
        std::stringstream ss(p);
        ss >> port;
    }

    std::vector< smt::shared_ptr<webserv::ServerBlock> >::iterator it;
    for (it = m_blocks.begin(); it != m_blocks.end(); it++) {
        FLOG_D("iterating " + a + " != " + (*it)->m_host);
        if ((*it)->m_port == port && (a.empty() || (*it)->m_host == a)) {
            FLOG_D("Getting server block corresponding to " + a + ":" + p);
            return (*it);
        }
    }
    FLOG_D("Getting default server block for host " + host_header);
    return (m_blocks[0]);
}

// int ServerSocket::bestServerBlockForRequest(
//     smt::shared_ptr<HTTPRequest>& request) {
//     std::string uri = request->getHeader("Host");

// if (uri.empty()) return (-1);
// if (startsWithIpAndPort(uri)) return bestServerBlockByIPAndPort(uri);
// if (startsWithIP(uri)) return bestServerBlockByIPAndPort(uri);
// if (startsWithServerName(uri)) return bestServerBlockByServerName(uri);
// return (-1);
// }

// int ServerSocket::bestServerBlockByIPAndPort(std::string& ipAndPort) {
//     char* tempIP = 0;
//     char* tempPort = 0;
//     sscanf(ipAndPort.c_str(), "%s:%s", tempIP, tempPort);

// std::string IP(tempIP);
// std::string port(tempPort);
// int         index = 0;

// for (std::vector< smt::shared_ptr<ServerBlock> >::iterator it =
//          m_blocks.begin();
//      it != m_blocks.end(); it++) {
//     if ((*it)->m_port == static_cast<unsigned>(atoi(port.c_str())) &&
//         ((*it)->m_host == IP || (*it)->m_host == "*")) {
//         return index;
//     }
//     index++;
// }
// LOG_D("No match found for ideal server block");
// return -1;
// }

// int ServerSocket::bestServerBlockByServerName(std::string& serverName) {
//     int index = 0;

// for (std::vector<smt::shared_ptr<ServerBlock>>::iterator it =
//          m_blocks.begin();
//      it != m_blocks.end(); it++) {
//     if ((*it)->m_server_name == serverName) { return index; }
//     index++;
// }
// LOG_D("No match found for ideal server block");
// return -1;
// }

// bool ServerSocket::startsWithServerName(std::string const& str) {
//     const std::string validChars =
//         "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-";
//     int const maxLabelLen = 63;

// size_t pos =
//     str.find_first_not_of(validChars); // Find first non-valid character
// if (pos == std::string::npos) {
//     // Cant start or end with a hyphen.
//     if (str.front() == '-' || str.back() == '-') { return false; }
//     return true;
// }
// else {
//     size_t labelStart = 0;
//     while (pos != std::string::npos) {
//         size_t labelLen = pos - labelStart;
//         if (labelLen == 0 || labelLen > maxLabelLen ||
//             str[labelStart] == '-' || str[pos - 1] == '-') {
//             return false;
//         }
//         labelStart = pos + 1;
//         pos = str.find('.', labelStart);
//     }
//     // Check the last label (after the last '.').
//     size_t lastLabelLen = str.length() - labelStart;
//     if (lastLabelLen == 0 || lastLabelLen > maxLabelLen ||
//         str[labelStart] == '-' || str.back() == '-') {
//         return false;
//     }
//     return true;
// }
// }

// bool ServerSocket::startsWithIP(std::string const& str) {
//     std::regex pattern(R"(^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})");
//     return std::regex_match(str, pattern);
// }

// bool ServerSocket::startsWithIpAndPort(std::string const& str) {
//     std::regex pattern(
//         "(\\d{1,3}\\.){3}\\d{1,3}:\\d+"); // Matches an IPv4 address with a
//         port
//                                           // number
//     return std::regex_match(str, pattern);
// }

// std::string ServerSocket::extractResource(std::string uri) {
//     // Check if the string starts with "http://" or "https://"
//     if (uri.compare(0, 7, "http://") == 0) { uri = uri.substr(7); }
//     else if (uri.compare(0, 8, "https://") == 0) { uri = uri.substr(8); }

// // Extract the server name
// size_t pos = uri.find('/');
// if (pos == std::string::npos) { return uri; }
// else { return ""; }
// }

} // namespace webserv
