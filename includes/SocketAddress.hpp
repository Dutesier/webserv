#ifndef SOCKET_ADDRESS_HPP
# define SOCKET_ADDRESS_HPP

/* ************************************************************************** */
/* Headers                                                                    */
/* ************************************************************************** */

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>

/* ************************************************************************** */
/* Class                                                                      */
/* ************************************************************************** */

namespace webserv {

class SocketAddress {

	public:

		/* Constructors and Destructors */
		SocketAddress(void);
		SocketAddress(int port, std::string host = "localhost",
					  int family = AF_INET);
		SocketAddress(SocketAddress const& src);

		~SocketAddress(void);

		/* Operator's Overload */
		SocketAddress&	operator=(SocketAddress const& rhs);

		/* Getters and Setters */
		struct sockaddr*	address(void) const;

		socklen_t	length(void) const;
		socklen_t*	length_ptr(void);
		std::string	host(void) const;

		int	port(void) const;
		int	family(void) const;

		bool	has_ipv4(void) const;
		bool	has_ipv6(void) const;

	private:

		/* Private Attributes */
		struct sockaddr_in*	addr;

		socklen_t	len;
		std::string	host_addr;

		bool	ipv4;
		bool	ipv6;

		int	port_nu;
		int	addr_family;

};

}	/* webserv */

#endif /* SOCKET_ADDRESS_HPP */
