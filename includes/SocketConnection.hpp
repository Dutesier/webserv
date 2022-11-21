#ifndef SOCKETCONNECTION_HPP
#define SOCKETCONNECTION_HPP

#include "Socket.hpp"
#include "SocketAddress.hpp"

#include <unistd.h>
#include <vector>

#ifndef READING_BUFFER
# define READING_BUFFER 1024
#endif

namespace webserv {

class SocketConnection : public webserv::Socket {

    public:

        /* Constructors and Destructors */
        SocketConnection(int fd, SocketAddress addr);
        SocketConnection(void);
        ~SocketConnection(void);

        /* Other Functions */
        bool close(void);

        std::string recv(void);
        bool        send(std::string message);
};

} // namespace webserv

#endif
