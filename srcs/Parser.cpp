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

Error*			Parser::get_error(void) const { return (this->error); }

unsigned int	Parser::get_port(void) const { return (this->port); }

std::string		Parser::get_address(void) const { return (this->address); }

std::string		Parser::get_root(void) const { return (this->root); }

/* ************************************************************************** */
/* Other Functions                                                            */
/* ************************************************************************** */

// This function returns true if the command var is valid so that it can be 
// successfully stored in this->listen. In case of error, the function returns
// false and this->error stops being nullptr to an instance of the Error class.
bool	Parser::listen_handler(std::vector<std::string> command) {

	std::string					address;
	std::stringstream			s;
	int							n;
	std::vector<std::string>	split_content;
	size_t						pos;
	
	// Basic error handling
	if (command.empty() || command[0] != "listen") {
		this->error = new Error();
		return (false);
	}
	if (command.size() < 2 ) {
		this->error = new Error();
		return (false);
	}
	// Check if there directive has both address and port by finding a ":"
	pos = command[1].find(":");
	if (pos != std::string::npos) {
		split_content.push_back(command[1].substr(0, pos));
		split_content.push_back(command[1].substr(pos + 1,command[1].size() - pos));
		// If we find another ":", the directive is wrong
		if (split_content[1].find(":") != std::string::npos) {
			this->error = new Error();
			return (false);
		}
		if (this->is_port_only(split_content[1]) == false) {
			this->error = new Error();
			return (false);
		}
		this->address = split_content[0];
		s << split_content[1];
		s >> n;
		if (n <= 0 || n > 65535) {
			this->error = new Error();
			return (false);
		}
		this->port = n;
		return (true);
	}
	// If no ":" are found, it means only an address or a port is delivered. Here we check if it is a port
	else if (this->is_port_only(command[1]) == true) {
		s << command[1];
		s >> n;
		if (n <= 0 || n > 65535) {
			this->error = new Error();
			return (false);
		}
		this->port = n;
		return (true);
	}
	// If it reached this point, we will assume it is an address
	else {
		this->address = command[1];
		return (true);
	}
	return (false);
}

// This function returns true if the command is valid so that it can be
// successfully stored in this->root. In case of error, the function returns
// false and this->error stops being nullptr to an instance of the Error class.
bool	Parser::root_handler(std::vector<std::string> command) {

	if (command.size() != 2 || command[0] != "root") {
		this->error = new Error();
		return (false);
	}
	this->root = command[1];
	return (true);
}


bool	Parser::is_port_only(std::string str) const{
	for ( size_t i = 0; i < str.size(); i++) {
		if (!isdigit(str[i]))
			return (false);
	}
	return (true);
}
