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
# include "Config.hpp"


/* ************************************************************************** */
/* Class                                                                      */
/* ************************************************************************** */

class ConfigParser : public Parser {

	public:

		/* Constructors and Destructors */
		ConfigParser(void);
		~ConfigParser(void);

		/* Getter and Setters */
		Config*	get_config(void) const;

		/* Other Functions */
		void	parse(std::ifstream& file);

		/* Handlers */
		bool	listen_handler(std::vector<std::string> commands);
		bool	root_handler(std::vector<std::string> commands);
		bool	server_handler(std::vector<std::string> commands);
		bool	end_block_handler(std::vector<std::string> commands);
		
	private:

		/* Private Functions */
		bool	valid_end(std::vector<std::string>* commands) const;
		bool	is_port(std::string port) const;


		/* Private Attribute */
		Config*	config;
};
