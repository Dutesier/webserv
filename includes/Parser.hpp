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

		/* Getters and Setters */
		unsigned int	get_listen(void) const;
		Error*			get_error(void) const;
		void			set_listen(unsigned int listen);
		void			set_error(Error* error);

		/* Other Functions */
		void	listen_handler(std::vector<std::string> command);


	private:

		/* Private Attributes */
		unsigned int	listen;
		Error*			error;

};

#endif /* PARSER_HPP */
