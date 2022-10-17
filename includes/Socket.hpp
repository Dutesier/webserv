#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <netinet/in.h>

/* ************************************************************************** */
/* Class                                                                      */
/* ************************************************************************** */

class Socket {

	public:

		/* Constructors and Destructors */
		Socket(int domain, in_port_t port);

		/* Getters and Setters */
		int	getFD(void) const;
		int	getName(void) const;

		/* Other Functions */
		bool	bind_to_port();

	private:

		/* Other Private Functions */
		void	init_address(in_port_t port, int domain);

		/* Private Attributes */
		int	fd;
		struct sockaddr_in	address;

};

#endif /* SOCKET_HPP */
