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

// Binds a socket to a sockaddr structure and sets its flags
// In other words, gives an fd a data structure
bool	SocketListener::bind_to_address(){
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

// Sets the port state to LISTEN and sets a BACKLOG max ammount of connections
// Every connection will have a specific fd/socket where we can communicate
bool	SocketListener::start_listening(){
	return (listen(this->fd, BACKLOG) == 0);
}

// Starts accepting connections. Accept is a blocking call that will wait for an incoming connection
// We can set a RCVTIMEO (timeout) value for the socket in order to prevent accept from blocking our program 
bool	SocketListener::accept_connections(){
	std::cout << "Accepting connections" << std::endl;

	// while (true) {
		SocketConnection* client = new SocketConnection;
		
		client->setFD(accept(this->fd, client->getAddress(), client->getAddrLenPointer()));
		if (client->getFD() >= 0){
			std::cerr << "Accept successful" << std::endl;
			connections.push_back(client);
			return true; // This would be commented
		} else {
			std::cerr << "Accept failed" << std::endl;
			delete client;
			return false; // This would be commented
		}
	// }
}

// TODO: Refactor this!
// Initializes a struct sockaddr_in called address and sets the IP, PORT, and connection type(TCP)
void	SocketListener::init_address(in_port_t port, int domain) {
	struct sockaddr_in* temp = new struct sockaddr_in;

	temp->sin_family = domain;
	temp->sin_port = htons(port);
	temp->sin_addr.s_addr = htonl(INADDR_ANY);
	this->address = reinterpret_cast<struct sockaddr *>(temp);
}
