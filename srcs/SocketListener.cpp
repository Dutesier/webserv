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
/* Other Functions (Socket setup)                                             */
/* ************************************************************************** */

// TODO: Refactor this!
// Initializes a struct sockaddr_in called address and sets the IP, PORT, and connection type(TCP)
void	SocketListener::init_address(in_port_t port, int domain) {
	struct sockaddr_in* temp = new struct sockaddr_in;

	temp->sin_family = domain;
	temp->sin_port = htons(port);
	temp->sin_addr.s_addr = htonl(INADDR_ANY);
	this->address = reinterpret_cast<struct sockaddr *>(temp);
}

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

/* ************************************************************************** */
/* Other Functions (I/O handling)                                             */
/* ************************************************************************** */

std::string SocketListener::read_from_connection(SocketConnection* connection){
	/* The following if statement is useful for testing but might also be useful in prod */
	if (connection == NULL){
		if (connections.size() > 0)
			connection = connections.at(0);
		else
			return ("No connection detected");
	}

	char buff[1024 + 1]; // This will probably not be a fized size
	size_t bytes_read;
	std::string temp;
	int maximum_iterations = 64; // This is equivalent to having a 64Kb packet (TCP max packet size)

	while ((bytes_read = recv(connection->getFD(), &buff, 1024, 0)) > 0) {
		if (!maximum_iterations)
			break;
		buff[bytes_read] = '\0';
		temp += buff;
		--maximum_iterations;
	}
	if (maximum_iterations == 64) // No loop was done
		return "No message in connection - RECV failed";
	return temp;
}

bool SocketListener::write_to_connection(SocketConnection* connection, std::string response){
	/* Same as above */
	if (connection == NULL){
		if (connections.size() > 0)
			connection = connections.at(0);
		else
			return ("No connection detected");
	}

	char* tempStr = new char[(response.size())];
	if (send(connection->getFD(), response.c_str(), response.size(), 0) < 0) {
		return false;
	}
	return true;
}