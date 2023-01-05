#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "TCPSocket.hpp"

namespace webserv {

class HTTPHandler {

    public:

        static void handle(TCPSocket* socket, int fd);

#ifndef GTEST_TESTING

    private:

#endif

        /* PImpl Object */
        struct m_impl;
};

struct HTTPHandler::m_impl {

        /* Constructor */
        m_impl(std::string req);
        ~m_impl(void);

        /* Other Methods */
        void execute(void);

        /* Member Attributes */
        HTTPRequest*  request;
        HTTPResponse* response;
};

} // namespace webserv

#endif /* HTTP_HANDLER_HPP */
