#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "SocketConnection.hpp"
# include <arpa/inet.h>

class Client {
public:
    Client(int domain, in_port_t port);
    ~Client();

    void send_message(std::string message);
    std::string receive_message(void);

private:
    SocketConnection connection;
};

#endif