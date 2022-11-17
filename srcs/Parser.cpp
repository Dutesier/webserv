#include "Parser.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

Parser::Parser( void ): fail(NULL) {}

Parser::~Parser( void ) {}

/* ************************************************************************** */
/* Other Functions                                                            */
/* ************************************************************************** */

//TODO: take comments out
std::vector<std::string>	Parser::split_line(std::string line) const {

	std::vector<std::string>	commands;
	char	*word;

	word = strtok(const_cast<char*>(line.c_str()), " \t");
	while (word) {
		commands.push_back(word);
		word = strtok(NULL, " \t");
	}

	return (commands);
}

Fail*	Parser::error(void) const {
	return (this->fail);
}
