#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "socket/SocketConnection.hpp"
#include "utils/smt.hpp"

#include <arpa/inet.h>

class Client : webserv::Socket {
    public:

        Client(in_port_t port) : webserv::Socket(port) {
            m_fd = socket(AF_INET, SOCK_STREAM, 0);
            connection = smt::make_shared(
                new webserv::SocketConnection(m_fd, address()));
            if (connect(connection->sockfd(), connection->address()->address(),
                        sizeof(*connection->address()->address())) < 0)
                throw(ConnectFailureException());
        }

        ~Client(void) { close(); }

        void close(void) {}

        void send_message(std::string message) { connection->send(message); }

        std::string receive_message(void) { return (connection->recv()); }

        /* Exceptions */
        struct ConnectFailureException : public std::exception {
                char const* what(void) const throw() {
                    return ("Client: failed to connect");
                }
        };

    private:

        smt::shared_ptr<webserv::SocketConnection> connection;
};

#endif
