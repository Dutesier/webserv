#ifndef SOCKETLISTENER_HPP
# define SOCKETLISTENER_HPP

# include "Socket.hpp"
# include "SocketConnection.hpp"
# include <vector>
# include <unistd.h>

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
		bool	bind(void);
		bool	start_listening(void);
        bool    accept_connections(void);
		void	shutdown_socket(void);
		void	close(void);

        std::string read_from_connection(SocketConnection* connection);
        bool        write_to_connection(SocketConnection* connection,
										std::string response);

	private:

		/* Other Private Functions */
        std::vector<SocketConnection*> connections;

};

} /* webserv */

#endif /* SOCKET_HPP */
