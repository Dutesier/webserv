#ifndef SOCKETLISTENER_HPP
# define SOCKETLISTENER_HPP

# include "Socket.hpp"
# include "SocketConnection.hpp"
# include <vector>

# ifndef BACKLOG
#  define BACKLOG 10
# endif

/* ************************************************************************** */
/* Class                                                                      */
/* ************************************************************************** */

class SocketListener: public Socket {

	public:

		/* Constructors and Destructors */
		SocketListener(int domain, in_port_t port);
		~SocketListener();

		/* Other Functions */
		bool	bind_to_address();
		bool	start_listening();
        bool    accept_connections();

        std::string read_from_connection(SocketConnection* connection);
        bool        write_to_connection(SocketConnection* connection, std::string response);

	private:
		/* Other Private Functions */
		void	init_address(in_port_t port, int domain);
        std::vector<SocketConnection*> connections;

};

#endif /* SOCKET_HPP */