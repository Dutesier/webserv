#include "ConfigParser.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

ConfigParser::ConfigParser( void ) : Parser(), config(new Config) {
	this->config->port = 80;
	this->config->address = "localhost";
	this->config->root = "/var/www/html";
	this->config->server = false;
	this->config->location = false;
}

ConfigParser::~ConfigParser( void ) { if (this->config) delete this->config; }

/* ************************************************************************** */
/* Getters and Setters                                                        */
/* ************************************************************************** */

struct Config*	ConfigParser::get_config(void) const { return (this->config); }

/* ************************************************************************** */
/* Other Functions                                                            */
/* ************************************************************************** */

void	ConfigParser::parse(std::string arg) {

	std::vector<std::string>	commands;

	std::string		line;
	unsigned int	i;

	// open config file
	std::ifstream	file;
	file.open(arg);
	if (!file.is_open()) {
		perror("failed to open");
		this->fail = new Fail("failed to open", arg);
		return ;
	}

	// read a line from a config file
	// convert it to a vector of string - commands
	i = 1;
	while (getline(file, line)) {
		if (!line.empty()) {

			commands = this->split_line(line);
			// handle commands
			if (commands[0] == "listen" && !listen_handler(commands))
				this->fail = new Fail(line, arg, i);
			else if (commands[0] == "root" && !root_handler(commands))
				this->fail = new Fail(line, arg, i);
			// else
			// 	this->fail = new Fail("unknown command", arg);

			// checking for errors
			if (this->fail)
				return ;
		}
		i++;
		commands.clear();
	}
}

// this function returns true if the last string of commands ends with in a ';'
// removing the ';' from commands if valid
bool	ConfigParser::valid_end(std::vector<std::string>* commands) const {
	if (commands->back().back() == ';') {
		commands->back().resize( commands->back().size() - 1);
		return (true);
	}
	return (false);
}

// This function returns true if 'port' is valid - only containes digits
// TODO: try and make this in a more cpp manner if possible
bool	ConfigParser::is_port(std::string port) const {
	for (size_t i = 0; i < port.size(); i++) {
		if (!isdigit(port[i])) return (false);
	}
	return (true);
}

// This function returns true if the command var is valid so that it can be 
// successfully stored in this->listen. In case of error, the function returns
// false and this->error stops being nullptr to an instance of the Error class.
bool	ConfigParser::listen_handler(std::vector<std::string> commands) {

	// checking if command is empty
	// or if commands starts with listen
	if (commands.empty() || commands[0] != "listen")
		return (false);
	// checking if command size is valid
	if (commands.size() < 2)
		return (false);
	// checking if command ends with ';'
	if (!this->valid_end(&commands))
		return (false);
	// checking if command is inside correct block
	if (!this->config->server)
		return (false);

	// checking listen directive syntax
	// possible syntaxes: listen port; listen address[:port]
	std::string	port;
	std::string	address;
	size_t	n = commands[1].find(":");
	if (n != std::string::npos) {
		address = commands[1].substr(0, n);
		port = commands[1].substr(n + 1);
	}
	else if (!this->is_port(commands[1]))
		address = commands[1];
	else
		port = commands[1];

	// storing and validating port and address
	if (!port.empty()) {
		std::stringstream	ss(port);
		ss >> this->config->port;
		if (this->config->port < 1 || this->config->port > 65535)
			return (false);
	}
	if (!address.empty()) this->config->address = address;

	return (true);
}

bool	ConfigParser::root_handler(std::vector<std::string> commands) {

	// checking if command is empty
	// or if commands starts with listen
	if (commands.empty() || commands[0] != "root")
		return (false);
	// checking if command size is valid
	if (commands.size() < 2)
		return (false);
	// checking if command ends with ';'
	if (!this->valid_end(&commands))
		return (false);
	// checking if command is inside correct block
	if (!this->config->server)
		return (false);

	// storing root
	this->config->root = commands[1];	

	return (true);
}

// this function handler location and server blocks.
// when a commands is the start of a block, the function only sets the block
// variable to true. when a commands is the end of a block the function sets
// the ending block to false - it finds the ending block hereditarily
bool	ConfigParser::block_handler(std::vector<std::string> commands) {

	if (commands.empty())
		return (false);
	// checking if command size is valid
	if (commands.size() > 2)
		return (false);

	// start of server block
	if ((commands[0] == "server" && commands[1] == "{")
		|| commands[0] == "server{" ) {
		if (this->config->server)
			return (false);
		this->config->server = true;
	}
	// start of server block
	else if ((commands[0] == "location" && commands[1] == "{")
		|| commands[0] == "location{" ) {
		if (this->config->location || !this->config->server)
			return (false);
		this->config->location = true;
	}
	// end of block
	else if (commands[0] == "}") {
		if (this->config->location)
			this->config->location = false;
		else if (this->config->server)
			this->config->server = false;
	}
	return (true);
}
