#ifndef SOCKETCONNECTION_HPP
#define SOCKETCONNECTION_HPP

#include "http/HTTPParser.hpp"
#include "socket/Socket.hpp"
#include "socket/SocketAddress.hpp"
#include "utils/Logger.hpp"

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>

#ifndef READING_BUFFER
# define READING_BUFFER 8192
#endif

namespace webserv {

class SocketConnection : public webserv::Socket {

    public:

        SocketConnection(int fd, smt::shared_ptr<SocketAddress> addr);
        SocketConnection(void);
        ~SocketConnection(void);

        void        close(void);
        std::string recv(void);
        void        send(std::string message);

		std::string getNextRequest(std::string req_str = "");

        struct CloseFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct SendFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct RecvFailureException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        HTTPParser m_parser;
};

} // namespace webserv

#endif
