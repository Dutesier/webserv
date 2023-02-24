#include "socket/ServerSocket.hpp"
#include <cstring>
#include <regex>


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
    std::string uri = request->getHeader("Host");

    if (uri.empty())
        return (-1);
    if (startsWithIpAndPort(uri))
        return bestServerBlockByIPAndPort(uri);
    if (startsWithIP(uri))
        return bestServerBlockByIPAndPort(uri);
    if (startsWithServerName(uri))
        return bestServerBlockByServerName(uri);
    return (-1);
}

int ServerSocket::bestServerBlockByIPAndPort(std::string& ipAndPort) {
    char* tempIP;
    char* tempPort;
    sscanf(ipAndPort.c_str(), "%s:%s", tempIP, tempPort);

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
    LOG_D("No match found for ideal server block");
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
    LOG_D("No match found for ideal server block");
    return -1;
}

bool ServerSocket::startsWithServerName(const std::string& str) {
    const std::string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-";
    const int maxLabelLen = 63;

    size_t pos = str.find_first_not_of(validChars);  // Find first non-valid character
    if (pos == std::string::npos) {
        // Cant start or end with a hyphen.
        if (str.front() == '-' || str.back() == '-') {
            return false;
        }
        return true;
    } else {
        size_t labelStart = 0;
        while (pos != std::string::npos) {
            size_t labelLen = pos - labelStart;
            if (labelLen == 0 || labelLen > maxLabelLen || str[labelStart] == '-' || str[pos - 1] == '-') {
                return false;
            }
            labelStart = pos + 1;
            pos = str.find('.', labelStart);
        }
        // Check the last label (after the last '.').
        size_t lastLabelLen = str.length() - labelStart;
        if (lastLabelLen == 0 || lastLabelLen > maxLabelLen || str[labelStart] == '-' || str.back() == '-') {
            return false;
        }
        return true;
    }
}

bool ServerSocket::startsWithIP(const std::string& str) {
    std::regex pattern(R"(^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})");
    return std::regex_match(str, pattern);
}

bool ServerSocket::startsWithIpAndPort(const std::string& str) {
    std::regex pattern("(\\d{1,3}\\.){3}\\d{1,3}:\\d+"); // Matches an IPv4 address with a port number
    return std::regex_match(str, pattern);
}

std::string ServerSocket::extractResource(std::string uri) {
    // Check if the string starts with "http://" or "https://"
    if (uri.compare(0, 7, "http://") == 0) {
        uri = uri.substr(7);
    } else if (uri.compare(0, 8, "https://") == 0) {
        uri = uri.substr(8);
    }

    // Extract the server name
    size_t pos = uri.find('/');
    if (pos == std::string::npos) {
        return uri;
    } else {
        return "";
    }
}


} // namespace webserv
