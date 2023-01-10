#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "socket/SocketConnection.hpp"
#include "utils/smt.hpp"

#include <arpa/inet.h>

class Client : webserv::Socket {
    public:

        Client(in_port_t port);
        ~Client(void);

        void                         close(void);
        void                         send_message(std::string message);
        smt::shared_ptr<HTTPRequest> receive_message(void);

        /* Exceptions */
        struct CloseFailureException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        smt::shared_ptr<webserv::SocketConnection> connection;
};

#endif
