#include "ConfigParser.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

ConfigParser::ConfigParser( void ) : Parser() {}

ConfigParser::~ConfigParser( void ) {}

/* ************************************************************************** */
/* Other Functions                                                            */
/* ************************************************************************** */
#include <cstdio>

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
			std::cout << "line: " << line << std::endl;
			commands = this->split_line(line);

			for ( size_t i = 0; i < commands.size(); i++)
				std::cout << "commands[" << i << "]: " << commands[1] << std::endl;

			// handle commands
			if (commands[0] == "listen" && !listen_handler(commands)) {
				std::cout << "here" << std::endl;
				this->fail = new Fail(line, arg, i);
			}
			else if (commands[0] == "root" && !root_handler(commands))
				this->fail = new Fail(line, arg, i);

			// checking for errors
			if (this->fail) {
				std::cout << "error: " << *this->fail << std::endl;
				return ;
			}
		}
		i++;
		commands.clear();
	}
}

bool	ConfigParser::listen_handler(std::vector<std::string> commands) {
	return (false);
}

bool	ConfigParser::root_handler(std::vector<std::string> commands) {
	return (false);
}
