#ifndef PARSER_HPP
# define PARSER_HPP

/* ************************************************************************** */
/* Headers                                                                    */
/* ************************************************************************** */

# include <vector>
# include <iostream>
# include <cstring>

# include "Fail.hpp"

/* ************************************************************************** */
/* Class                                                                      */
/* ************************************************************************** */

class Parser {

	public:

		/* Constructors and Destructors */
		Parser(void);
		virtual ~Parser(void);

		/* Other Functions */
		std::vector<std::string>	split_line(std::string line) const;
		Fail*	error(void) const;
		virtual void	parse(std::string arg) = 0;

	protected:

		Fail*	fail;

};

#endif /* PARSER_HPP */
