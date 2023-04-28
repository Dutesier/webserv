#include "socket/ServerSocket.hpp"

namespace webserv {

ServerSocket::ServerSocket(smt::shared_ptr<ServerAddress>& addr)
    : m_sockFd(-1), m_addr(addr) {
    LOG_D("Created " + toString());
}

ServerSocket::~ServerSocket(void) {
    close();
    LOG_D("Destroying " + toString());
}

int ServerSocket::getSockFd(void) const { return (m_sockFd); }

int ServerSocket::getPort(void) const { return (m_addr->getPort()); }

std::string ServerSocket::getHost(void) const { return (m_addr->getHost()); }

sa_family_t ServerSocket::getFamily(void) const {
    return (m_addr->getFamily());
}

int ServerSocket::getType(void) const { return (m_addr->getType()); }

sockaddr* ServerSocket::getAddress(void) const {
    return (m_addr->getAddress());
}

socklen_t ServerSocket::getLength(void) const { return (m_addr->getLength()); }

void ServerSocket::socket(void) {
    if ((m_sockFd = ::socket(m_addr->getFamily(), m_addr->getType(), 0)) < 0) {
        LOG_E(toString() + " failure in ::socket()");
        throw SocketFailureException();
    }
}

smt::shared_ptr<SocketConnection> ServerSocket::getConnection(int connectFd) {

    std::map<int, smt::shared_ptr<SocketConnection> >::iterator it;
    it = m_connections.find(connectFd);
    if (it == m_connections.end()) {
        LOG_E(toString() + " has no connection with fd " +
              std::to_string(connectFd));
        throw NoSuchConnectionException();
    }
    return ((*it).second);
}

std::map< int, smt::shared_ptr<SocketConnection> >
    ServerSocket::getConnections(void) {
    return (m_connections);
}

void ServerSocket::bind(void) {
    if (::bind(m_sockFd, m_addr->getAddress(), m_addr->getLength()) == -1) {
        LOG_E(toString() + " failure in ::bind()");
        throw BindFailureException();
    }
}

void ServerSocket::listen(void) {
    if (::listen(m_sockFd, BACKLOG) == -1) {
        LOG_E(toString() + " failure in ::listen()");
        throw ListenFailureException();
    }
}

void ServerSocket::setsockopt(int level, int optname, void const* optval,
                              socklen_t optlen) {
    if (::setsockopt(m_sockFd, level, optname, optval, optlen) == -1) {
        LOG_E(toString() + " failure in ::setsockopt()");
        throw SetOptFailureException();
    }
}

int ServerSocket::accept(void) {

    sockaddr* connectAddr = new sockaddr;
    socklen_t len = sizeof(connectAddr);

    int connectFd = ::accept(m_sockFd, connectAddr, &len);

    if (connectFd < 0) {
        LOG_E(toString() + " failure in ::accept()");
        throw(AcceptFailureException());
    }

    smt::shared_ptr< SocketConnection > sockConnect(new SocketConnection(
        connectFd, reinterpret_cast<sockaddr_in*>(connectAddr), len));

    m_connections[connectFd] = sockConnect;
    return (connectFd);
}

void ServerSocket::close(void) {

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

void ServerSocket::close(int connectFd) {

    std::map<int, smt::shared_ptr<SocketConnection> >::iterator it;
    it = m_connections.find(connectFd);

    if (it == m_connections.end()) {
        LOG_E(toString() + " failure in ::close(): no such connection");
        throw(NoSuchConnectionException());
    }

    m_connections.erase(it);
}

std::string ServerSocket::recv(int connectFd) {

    std::map<int, smt::shared_ptr<SocketConnection> >::iterator it;
    it = m_connections.find(connectFd);

    if (it == m_connections.end()) {
        LOG_E(toString() + " failure in ::recv(): no such connection");
        throw(NoSuchConnectionException());
    }

    std::string request;
    try {
        request = m_connections[connectFd]->recv();
    }
    catch (SocketConnection::RecvFailureException& e) {
        LOG_E(toString() + " failure in ::recv(): " + e.what());
        throw RecvFailureException();
    }
    return (request);
}

void ServerSocket::send(int connectFd, std::string const& response) {

    std::map<int, smt::shared_ptr<SocketConnection> >::iterator it;
    it = m_connections.find(connectFd);

    if (it == m_connections.end()) {
        LOG_E(toString() + " failure in ::send(): no such connection");
        throw(NoSuchConnectionException());
    }

    try {
        m_connections[connectFd]->send(response);
    }
    catch (SocketConnection::SendFailureException& e) {
        LOG_E(toString() + " failure in ::send(): " + e.what());
        throw SendFailureException();
    }
}

std::string ServerSocket::toString(void) const {
    std::ostringstream oss;
    oss << "Server Socket " << getPort() << ":" << getHost();
    return (oss.str());
}

char const* ServerSocket::SocketFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::socket()");
}

char const* ServerSocket::BindFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::bind()");
}

char const* ServerSocket::ListenFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::listen()");
}

char const* ServerSocket::SetOptFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::setsockopt()");
}

char const* ServerSocket::AcceptFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::accept()");
}

char const* ServerSocket::CloseFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::close()");
}

char const* ServerSocket::SendFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::send()");
}

char const* ServerSocket::RecvFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::recv()");
}

char const* ServerSocket::NoSuchConnectionException::what(void) const throw() {
    return ("Server Socket: no such connection");
}

} // namespace webserv