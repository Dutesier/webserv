#include "socket/SocketConnection.hpp"

namespace webserv {

SocketConnection::SocketConnection(int sockFd, sockaddr_in* addr,
                                   socklen_t addrLen)
    : m_parser(smt::make_shared<HTTPParser>(new HTTPParser())),
      m_addr(new ConnectionAddress(addr, addrLen)), m_sockFd(sockFd) {
    LOG_D("Created " + toString());
}

SocketConnection::~SocketConnection(void) {
    close();
    LOG_D("Destroying " + toString());
}

int SocketConnection::getSockFd(void) const { return (m_sockFd); }

int SocketConnection::getPort(void) const { return (m_addr->getPort()); }

std::string SocketConnection::getHost(void) const {
    return (m_addr->getHost());
}

sa_family_t SocketConnection::getFamily(void) const {
    return (m_addr->getFamily());
}

sockaddr* SocketConnection::getAddress(void) const {
    return (m_addr->getAddress());
}

socklen_t SocketConnection::getLength(void) const {
    return (m_addr->getLength());
}

void SocketConnection::close(void) {
    if (m_sockFd == -1) {
        LOG_D(toString() + " is already closed");
        return;
    }
    if (::close(m_sockFd) == -1) {
        LOG_E(toString() + " failure in ::close()");
        throw CloseFailureException();
    }
    m_sockFd = -1;
}

std::string SocketConnection::recv(void) {
    char buffer[READING_BUFFER + 1];
    int  nBytes = ::recv(m_sockFd, buffer, READING_BUFFER, 0);
    if (nBytes == -1) {
        LOG_E(toString() + " failure in ::recv()");
        throw RecvFailureException();
    }
    return (std::string(buffer, nBytes));
}

void SocketConnection::send(std::string response) {
    if (::send(m_sockFd, response.c_str(), response.size(), 0) < 0) {
        LOG_E(toString() + " failure in ::send()");
        throw SendFailureException();
    }
}

std::string SocketConnection::toString(void) const {
    std::ostringstream oss;
    oss << "Socket Connection " << getPort() << ":" << getHost();
    return (oss.str());
}

char const* SocketConnection::CloseFailureException::what(void) const throw() {
    return ("Socket Connection: failure in ::close()");
}

char const* SocketConnection::SendFailureException::what(void) const throw() {
    return ("Socket Connection: failure in ::send()");
}

char const* SocketConnection::RecvFailureException::what(void) const throw() {
    return ("Socket Connection: failure in ::recv()");
}

} // namespace webserv