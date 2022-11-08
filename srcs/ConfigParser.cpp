#include "ConfigParser.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

ConfigParser::ConfigParser( void ) : Parser(), config(new Config) {}

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
		this->fail = new Fail("failed to open", arg);
		return ;
	}

	i = 1;
	// read a line from a config file
	while (getline(file, line)) {

		if (!line.empty()) {
			// convert it to a vector of string - commands
			commands = this->split_line(line);
			// handle commands
			if (commands[0] == "listen" && listen_handler(commands)) ;
			else if (commands[0] == "root" && root_handler(commands)) ;
			else if ((commands[0] == "server" || commands[0] == "server{")
					  && server_handler(commands)) ;
			else if (commands[0] == "}" && end_block_handler(commands)) ;
			else this->fail = new Fail(line, arg, i);

			// checking for errors
			if (this->fail) return ;
		}
		i++;
		commands.clear();
	}
	// here we need to check if blocks were correctly closed
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

	for (size_t i = 0; i < port.size(); i++)
		if (!isdigit(port[i])) return (false);
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
	// or if commands starts with root
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

// this function handles server blocks - it validates if the start of a server
// block is valid
bool	ConfigParser::server_handler(std::vector<std::string> commands) {

	// checking if command is empty
	if (commands.empty())
		return (false);
	// checking if command size is valid
	if (commands.size() > 2)
		return (false);

	// start of server block
	if (commands[0] == "server{" || (commands.size() > 1
		&& commands[0] == "server" &&commands[1] == "{")) {
		if (this->config->server) return (false);
		this->config->server = true;
	}
	else return (false);

	return (true);
}

// This function handler commands that contain a '}'. It closes a block -
// either location or server blocks
bool	ConfigParser::end_block_handler(std::vector<std::string> commands) {

	// checking if command is empty
	// or if commands starts with listen
	if (commands.empty() || commands[0] != "}")
		return (false);
	// checking if command size is valid
	if (commands.size() != 1)
		return (false);

	if (this->config->location)
		this->config->location = false;
	else if (this->config->server)
		this->config->server = false;
	else
		return (false);
	return (true);
}
