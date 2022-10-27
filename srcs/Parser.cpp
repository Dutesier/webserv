#include "Parser.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

Parser::Parser( void ) {}

Parser::~Parser( void ) {
	if (this->error)
		delete this->error;
}

/* ************************************************************************** */
/* Getters and Setters                                                        */
/* ************************************************************************** */

unsigned int	Parser::get_listen(void) const { return (this->listen); }

Error*	Parser::get_error(void) const { return (this->error); }

void	Parser::set_listen(unsigned int listen) { this->listen = listen; }

void	Parser::set_error(Error* error) {

	if ( this->error )
		delete this->error;
	this->error = error;
}

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
		this->set_error(new Error());
		return (false);
	}

	if (command.size() < 2 ) {
		this->set_error(new Error());
		return (false);
	}

	s << command[1];
	s >> port;
	if (port <= 0) {
		this->set_error(new Error());
		return (false);
	}

	this->set_listen(port);
	return (true);
}
