#ifndef CONFIGSETUP_HPP
# define CONFIGSETUP_HPP

# include "Fail.hpp"
# include "Logger.hpp"
# include "ConfigParser.hpp"
# include <iostream>
# include <fstream>

/*!
 * \brief A class that works as a factory, generating a Config object
 * 
 * Class for setting up a Config instance from the config file, uses a ConfigParser
 * and checks argv for a provided file. If none is given it returns a NULL pointer
 * of type Config*. Else, a valid Config* is provided.
 * 
 * \param argc Argument count from main
 * \param argv Arguments from main
 */
class ConfigSetup {
public:
    explicit ConfigSetup(int argc, char *argv[]);
    ~ConfigSetup();

    //! The Config generator.
    /*!
        Returns a pointer to a Config heap allocated instance via subsequent calls
        to ConfigParser. You must handle this memory as it will NOT be deleted by the instantiator.
    */
    Config* get_config();

private:
    Config* config_from_argument(std::string argument);
    Config* config_from_default();

    Config* get_config_from_file(std::ifstream& configFile);

    Config*         config = NULL;
    
    int     argc;
    char**   argv;
    Fail            error;
};

#endif
