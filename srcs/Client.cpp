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

bool Client::close() { return ::close(this->fd); }

bool Client::send_message(std::string message) {
    return connection->send(message);
}

std::string Client::receive_message(void) { return (connection->recv()); }
