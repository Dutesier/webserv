#include "Client.hpp"

Client::Client(in_port_t port) : webserv::Socket(port) {
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    connection = smt::shared_ptr<webserv::SocketConnection>(
        new webserv::SocketConnection(this->fd, this->address()));
    if (connect(connection->sockfd(), connection->address().address(),
                sizeof(*connection->address().address())) < 0)
        perror("Client: Connect");
}

Client::~Client() { this->close(); }

void Client::close() {
    if (this->fd == -1) return;
    if
        ::close(this->fd < 0) throw(CloseFailureException());
    this->fd = -1;
}

bool Client::send_message(std::string message) {
    return connection->send(message);
}

std::string Client::receive_message(void) { return (connection->recv()); }

char const* Client::CloseFailureException::what(void) const throw() {
    return ("Client failure in close()");
}
