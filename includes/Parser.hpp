#ifndef PARSER_HPP
# define PARSER_HPP

/* ************************************************************************** */
/* Headers                                                                    */
/* ************************************************************************** */

# include <vector>
# include <iostream>
# include <sstream>

# include "Error.hpp"

/* ************************************************************************** */
/* Class                                                                      */
/* ************************************************************************** */

class Parser {

	public:

		/* Constructors and Destructors */
		Parser(void);
		~Parser(void);

		/* Getters and Setters */
		unsigned int	get_port(void) const;
		std::string		get_address(void) const;
		std::string		get_root(void) const;
		Error*			get_error(void) const;

		/* Other Functions */
		bool	listen_handler(std::vector<std::string> command);
		bool	root_handler(std::vector<std::string> command);

	private:

		/* Private Attributes */
		unsigned int	port;
		std::string		address;
		std::string 	root;
		Error*			error;

		bool	is_port_only(std::string str) const;
};

#endif /* PARSER_HPP */
