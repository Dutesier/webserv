#include "Client.hpp"

Client::Client(in_port_t port) : webserv::Socket(port) {

    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    connection = smt::shared_ptr<webserv::SocketConnection>(
        new webserv::SocketConnection(m_fd, address()));
    if (connect(connection->sockfd(), connection->address()->address(),
                sizeof(*connection->address()->address())) < 0)
        perror("Client: Connect");
}

Client::~Client() { close(); }

void Client::close() {}

void Client::send_message(std::string message) { connection->send(message); }

std::string Client::receive_message(void) { return (connection->recv()); }

char const* Client::CloseFailureException::what(void) const throw() {
    return ("Client failure in close()");
}
