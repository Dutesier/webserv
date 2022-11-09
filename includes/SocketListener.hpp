#ifndef SOCKETLISTENER_HPP
# define SOCKETLISTENER_HPP

# include "Socket.hpp"
# include "SocketConnection.hpp"
# include <vector>
# include <unistd.h>

// TODO: make this either specific to each socket or to each server
// That way we can tests specific backlogs
# ifndef BACKLOG
#  define BACKLOG 10
# endif

/* ************************************************************************** */
/* Class                                                                      */
/* ************************************************************************** */

namespace webserv {

class SocketListener : public Socket {

	public:

		/* Constructors and Destructors */
		// SocketListener(int domain, in_port_t port);
		SocketListener(int port, std::string host = "localhost",
				 	   int family = AF_INET);
		~SocketListener(void);

		/* Other Functions */
		// TODO: understand what should be const and whatnot
		bool	bind(void);
		bool	listen(void);
		bool	setsockopt(int level, int optname, const void *optval,
				  		   socklen_t optlen);
		//TODO: change return val
        bool	accept(void);
		bool	shutdown(int how);
		bool	close(void);

        std::string recv(SocketConnection* connection);
        bool        send(SocketConnection* connection,
						 std::string response);

	private:

		// /* Other Private Functions */
  		// std::vector<SocketConnection*> connections;

};

} /* webserv */

#endif /* SOCKET_HPP */
