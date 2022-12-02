#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

#include "ServerConfig.hpp"
#include "TCPSocket.hpp"

namespace webserv {

class ServerSocket : public TCPSocket {

	public:

		/* Constructor and Destructor */
		ServerSocket(ServerConfig* config);
		~ServerSocket(void);

	private:

		/* Private Attributes */
        ServerConfig* configs;

};

}

#endif /* SERVER_SOCKET_HPP */
