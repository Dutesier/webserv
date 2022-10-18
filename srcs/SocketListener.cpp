#include "SocketListener.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

SocketListener::SocketListener(int domain, in_port_t port) {
	this->fd = socket(domain, SOCK_STREAM, 0);
	init_address(port, domain);
}

SocketListener::~SocketListener( void ) {
	if (this->fd != -1)
		close(this->fd);
	if (this->address)
		delete(this->address);
	for (std::vector<SocketConnection *>::iterator it = connections.begin(); it < connections.end(); ++it){
		delete *it;
	}
	connections.clear();
}



/* ************************************************************************** */
/* Other Functions                                                            */
/* ************************************************************************** */

bool	SocketListener::bind_to_port(){
	const int enable = 1;
	struct timeval timeout;      
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    	std::cerr << ("setsockopt(SO_REUSEADDR) failed") << std::endl;
	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0)
    	std::cerr << ("setsockopt(SO_REUSEPORT) failed") << std::endl;
	if (setsockopt(this->fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    	std::cerr << ("setsockopt(SO_RCVTIMEO) failed") << std::endl;
	
	return ( bind(this->fd, this->address, sizeof(*address)) == 0);
}


bool	SocketListener::start_listening(){
	return (listen(this->fd, BACKLOG) == 0);
}

bool	SocketListener::accept_connections(){
	int connection_fd = -1;
	std::cout << "Accepting connections" << std::endl;

	// while (true) {
		SocketConnection* client = new SocketConnection;
		
		connection_fd = accept(this->fd, client->getAddress(), client->getAddrLenPointer());
		std::cout << "Reached this" << std::endl;
		client->setFD(connection_fd);
		if (client->getFD() >= 0){
			std::cerr << "Accept successful" << std::endl;
			connections.push_back(client);
			return true;
		} else {
			std::cerr << "Accept failed" << std::endl;
			delete client;
			return false;
		}
	// }
}

// TODO: Refactor this!
void	SocketListener::init_address(in_port_t port, int domain) {
	struct sockaddr_in* temp = new struct sockaddr_in;
	temp->sin_family = domain;
	temp->sin_port = htons(port);
	temp->sin_addr.s_addr = htonl(INADDR_ANY);
	this->address = reinterpret_cast<struct sockaddr *>(temp);
}

/* ************************************************************************** */
/* Class Exceptions                                                           */
/* ************************************************************************** */
