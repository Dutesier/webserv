#ifndef SOCKETCONNECTION_HPP
# define SOCKETCONNECTION_HPP

# include "Socket.hpp"
# include "SocketAddress.hpp"

# ifndef READING_BUFFER
#  define READING_BUFFER 1024
# endif

namespace webserv {

class SocketConnection: public webserv::Socket {
public:
    SocketConnection();
    ~SocketConnection();

	public:

		SocketConnection(int fd, SocketAddress addr);
		~SocketConnection(void);

		bool		close(void);

        std::string	recv(void);
        bool		send(std::string message);

};

} /* webserv */

#endif
