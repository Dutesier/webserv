#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "TCPSocket.hpp"

namespace webserv {

class HTTPHandler {

    public:

        static void handle(TCPSocket* socket, int fd);

#ifndef GTEST_TESTING

    private:

#endif

        /* PImpl Object */
        struct impl;
};

struct HTTPHandler::impl {
        impl(TCPSocket* socket, int fd);

        TCPSocket*        socket;
        SocketConnection* client;
};

} // namespace webserv

#endif /* HTTP_HANDLER_HPP */
