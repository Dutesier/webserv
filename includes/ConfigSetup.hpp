#ifndef CONFIGSETUP_HPP
# define CONFIGSETUP_HPP

# include "Error.hpp"
# include "Logger.hpp"
# include <iostream>
# include <fstream>

struct Config {};

// Class for setting up the config class from the config file
class ConfigSetup {
public:
    explicit ConfigSetup(int argc, char *argv[]);
    ~ConfigSetup();

    Config* get_config();

private:
    Config* config_from_argument(std::string argument);
    Config* config_from_default();
    Config* get_config_from_file(std::ifstream& configFile);

    Config* config = NULL;
    Error error;
};

#endif