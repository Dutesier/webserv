#ifndef SOCKETCONNECTION_HPP
# define SOCKETCONNECTION_HPP

/* ************************************************************************** */
/* Headers                                                                    */
/* ************************************************************************** */

# include "Socket.hpp"
# include "SocketAddress.hpp"

# include <vector>
# include <unistd.h>

# ifndef READING_BUFFER
#  define READING_BUFFER 1024
# endif

/* ************************************************************************** */
/* Class                                                                      */
/* ************************************************************************** */

namespace webserv {

class SocketConnection: public webserv::Socket {

	public:

		/* Constructors and Destructors */
		SocketConnection(int fd, SocketAddress addr);
		SocketConnection();
		~SocketConnection(void);

		/* Other Functions */
		bool		close(void);

        std::string	recv(void);
        bool		send(std::string message);

};

} /* webserv */

#endif
