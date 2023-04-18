#include "socket/SocketConnection.hpp"

namespace webserv {

SocketConnection::SocketConnection(int fd, smt::shared_ptr<SocketAddress> addr)
    : Socket(fd, addr, SOCK_STREAM)
    , m_parser(smt::make_shared<HTTPParser>(new HTTPParser())) {}

SocketConnection::SocketConnection()
    : Socket()
    , m_parser(smt::make_shared<HTTPParser>(new HTTPParser())) {}

SocketConnection::~SocketConnection(void) { this->close(); }

void SocketConnection::close(void) {

    if (m_fd == -1) { return; }
    if (::close(m_fd) < 0) { throw(CloseFailureException()); }
    m_fd = -1;
}

// void data_to_buff(char* data, char *buff) {
//     for (int i = 0; i < READING_BUFFER; ++i) {
//         buff[i] = data[i];
//         data[i] = 0;
//     }
//     buff[READING_BUFFER] = '\0';
// }

// void buff_to_data(char* buff, char* data) {
//     for (int i = 0; buff[i] != '\0'; ++i){
//         data[i] = buff[i];
//     }
// }

// Handle incomplete (even when just first word)

// TODO: find smarter ways to get buf
std::string SocketConnection::recv(void) {

    char buff[READING_BUFFER + 1];
    int  bytes_read;

    bytes_read = ::recv(m_fd, &buff, READING_BUFFER, 0);
    if (bytes_read < 0) { throw(SendFailureException()); }
    if (bytes_read == 0) { return (""); }

    return (std::string(buff, bytes_read));
}

void SocketConnection::send(std::string message) {

    if (::send(m_fd, message.c_str(), message.size(), 0) < 0) {
        throw(SendFailureException());
    }
}

char const* SocketConnection::CloseFailureException::what(void) const throw() {
    return ("webserv::SocketConnection failure in close()");
}

char const* SocketConnection::SendFailureException::what(void) const throw() {
    return ("webserv::SocketConnection failure in send()");
}

char const* SocketConnection::RecvFailureException::what(void) const throw() {
    return ("webserv::SocketConnection failure in recv()");
}
} // namespace webserv
