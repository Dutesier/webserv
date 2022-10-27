#include "Parser.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

Parser::Parser( void ) {}

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

void	Parser::listen_handler(std::vector<std::string> command) {
	int		port;
	std::stringstream	s;

	if (command.empty() || command[0] != "listen")
		return ( this->set_error(new Error()));
	if (command.size() < 2 )
		return ( this->set_error(new Error()));
	s << command[1];
	s >> port;
	if (port <= 0)
		return ( this->set_error(new Error()));
	this->set_listen( static_cast<unsigned int>(port));
}
