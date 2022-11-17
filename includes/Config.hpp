#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>

struct Config {
	Config(void);

	unsigned int	port;
	std::string		address;
	std::string		root;
	bool			server;
	bool			location;
};

#endif