#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "SocketConnection.hpp"
#include "smt.hpp"

#include <arpa/inet.h>

class Client : webserv::Socket {
    public:

        Client(in_port_t port);
        ~Client(void);

        void        close(void);
        bool        send_message(std::string message);
        std::string receive_message(void);

        /* Exceptions */
        struct CloseFailureException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        smt::shared_ptr<webserv::SocketConnection> connection;
};

#endif
