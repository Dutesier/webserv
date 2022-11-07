/* ************************************************************************** */
/* Headers                                                                    */
/* ************************************************************************** */

# include <cstdio>
# include <iostream>
# include <fstream>
# include <vector>
# include <sstream>

# include "Parser.hpp"
# include "Fail.hpp"

struct Config {
	unsigned int	port;
	std::string		address;
	std::string		root;
};

/* ************************************************************************** */
/* Class                                                                      */
/* ************************************************************************** */

struct	config;
class ConfigParser : public Parser {

	public:

		/* Constructors and Destructors */
		ConfigParser(void);
		~ConfigParser(void);

		/* Getter and Setters */
		struct Config*	get_config(void) const;

		/* Other Functions */
		void	parse(std::string arg);
		bool	listen_handler(std::vector<std::string> commands);
		bool	root_handler(std::vector<std::string> commands);

	private:

		/* Private Functions */
		bool	valid_end(std::vector<std::string>* commands) const;
		bool	is_port(std::string port) const;

		/* Private Attribute */
		struct Config*	config;
};
