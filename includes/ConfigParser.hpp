/* ************************************************************************** */
/* Headers                                                                    */
/* ************************************************************************** */

# include <iostream>
# include <vector>
# include <fstream>

# include "Parser.hpp"
# include "Fail.hpp"

/* ************************************************************************** */
/* Class                                                                      */
/* ************************************************************************** */

class ConfigParser : public Parser {

	public:

		/* Constructors and Destructors */
		ConfigParser(void);
		~ConfigParser(void);

		/* Other Functions */
		void	parse(std::string arg);
		bool	listen_handler(std::vector<std::string> commands);
		bool	root_handler(std::vector<std::string> commands);

};
