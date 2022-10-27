#include "Parser.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

Parser::Parser( void ) : port(80), error(nullptr) {}

Parser::~Parser( void ) {
	if (this->error)
		delete this->error;
}

/* ************************************************************************** */
/* Getters and Setters                                                        */
/* ************************************************************************** */

unsigned int	Parser::get_listen(void) const { return (this->port); }

Error*	Parser::get_error(void) const { return (this->error); }

std::string	Parser::get_root(void) const { return (this->root); }

/* ************************************************************************** */
/* Other Functions                                                            */
/* ************************************************************************** */

// This function returns true if the command var is valid so that it can be 
// successfully stored in this->listen. In case of error, the function returns
// false and this->error stops being nullptr to an instance of the Error class.
bool	Parser::listen_handler(std::vector<std::string> command) {

	int		port;
	std::stringstream	s;

	if (command.empty() || command[0] != "listen") {
		this->error = new Error();
		return (false);
	}

	if (command.size() < 2 ) {
		this->error = new Error();
		return (false);
	}

	s << command[1];
	s >> port;
	if (port <= 0) {
		this->error = new Error();
		return (false);
	}

	this->port = port;
	return (true);
}

// This function returns true if the command is valid so that it can be
// successfully stored in this->root. In case of error, the function returns
// false and this->error stops being nullptr to an instance of the Error class.
bool	Parser::root_handler(std::vector<std::string> command) {

	std::string root;
	if (command.size() != 2 || command[0] != "root") {
		this->error = new Error();
		return (false);
	}
	this->root = command[1];
	return (true);
}
