#include "Parser.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

Parser::Parser( void ) : listen(80), error(nullptr) {}

Parser::~Parser( void ) {
	if (this->error)
		delete this->error;
}

/* ************************************************************************** */
/* Getters and Setters                                                        */
/* ************************************************************************** */

unsigned int	Parser::get_listen(void) const { return (this->listen); }

Error*	Parser::get_error(void) const { return (this->error); }

/* ************************************************************************** */
/* Other Functions                                                            */
/* ************************************************************************** */

// This function returns true if the command var is valid and was successfully
// stored in this->listen. In case of error, the function returns false and
// this->error stops being nullptr to an instance of the Error class.
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

	this->listen = port;
	return (true);
}
