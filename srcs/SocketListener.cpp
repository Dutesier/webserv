#include "SocketListener.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

namespace webserv {

SocketListener::SocketListener(int port, std::string host, int family)
	: Socket(port, host, family, SOCK_STREAM) {
	this->fd = socket(family, this->socktype, 0);
}

SocketListener::~SocketListener( void ) {

	this->close();
	// for (std::vector<SocketConnection *>::iterator it = connections.begin(); it < connections.end(); ++it){
	// 	delete *it;
	// }
	// connections.clear();
}


/* ************************************************************************** */
/* Other Functions (Socket setup)                                             */
/* ************************************************************************** */

// Binds a socket to a sockaddr structure and sets its flags
// In other words, gives an fd a data structure
bool	SocketListener::bind(void) {
	return (::bind(this->fd, this->addr->address(), this->addr->length()) == 0);
}
void	SocketListener::close(void) {
	if (this->fd > 0) ::close(this->fd);
	this->fd = -1;
}

// bool	SocketListener::bind_to_address(){
	// const int enable = 1;
	// struct timeval timeout;      
 //    timeout.tv_sec = 10;
 //    timeout.tv_usec = 0;
	// if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
 //    	std::cerr << ("setsockopt(SO_REUSEADDR) failed") << std::endl;
	// if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0)
 //    	std::cerr << ("setsockopt(SO_REUSEPORT) failed") << std::endl;
	// if (setsockopt(this->fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
 //    	std::cerr << ("setsockopt(SO_RCVTIMEO) failed") << std::endl;
// 	
// 	return ( bind(this->fd, this->address, sizeof(*address)) == 0);
// }
//
// // Sets the port state to LISTEN and sets a BACKLOG max ammount of connections
// // Every connection will have a specific fd/socket where we can communicate
// bool	SocketListener::listen(){
// 	return (::listen(this->fd, BACKLOG) == 0);
// }
//
// // Starts accepting connections. Accept is a blocking call that will wait for an incoming connection
// // We can set a RCVTIMEO (timeout) value for the socket in order to prevent accept from blocking our program 
// bool	SocketListener::accept_connections(){
// 	std::cout << "Accepting connections" << std::endl;
//
// 	// while (true) {
// 		SocketConnection* client = new SocketConnection;
// 		
// 		client->set_fd(accept(this->fd, client->get_address(), client->get_addr_len_ptr()));
// 		if (client->get_fd() >= 0){
// 			std::cerr << "Accept successful" << std::endl;
// 			connections.push_back(client);
// 			return true; // This would be commented
// 		} else {
// 			std::cerr << "Accept failed" << std::endl;
// 			delete client;
// 			return false; // This would be commented
// 		}
// 	// }
// }
//
// /* ************************************************************************** */
// /* Other Functions (I/O handling)                                             */
// /* ************************************************************************** */
//
// std::string SocketListener::read_from_connection(SocketConnection* connection){
// 	/* The following if statement is useful for testing but might also be useful in prod */
// 	if (connection == NULL){
// 		if (connections.size() > 0)
// 			connection = connections.at(0);
// 		else
// 			return ("No connection detected");
// 	}
//
// 	return connection->read_connection();
// }
//
// bool SocketListener::write_to_connection(SocketConnection* connection, std::string response){
// 	/* Same as above */
// 	if (connection == NULL){
// 		if (connections.size() > 0)
// 			connection = connections.at(0);
// 		else
// 			return ("No connection detected");
// 	}
//
// 	return connection->write_connection(response);
// }

} /* webserv */
