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

		/* Need to be implemented */
		virtual void	parse(std::ifstream& file) = 0;

	protected:

		Fail*	fail = NULL;

};

#endif /* PARSER_HPP */
