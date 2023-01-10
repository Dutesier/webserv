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

#ifndef MAX_BODY_SIZE
# define MAX_BODY_SIZE 2147483648
#endif

namespace webserv {

class SocketConnection : public webserv::Socket {

    public:

        /* Constructors and Destructors */
        SocketConnection(int fd, SocketAddress addr);
        SocketConnection(void);
        ~SocketConnection(void);

        /* Other Functions */
        void close(void);

        smt::shared_ptr<HTTPRequest> recv(void);
        void                         send(std::string message);

        /* Exceptions */
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

        HTTPParser parser;
};

} // namespace webserv

#endif
