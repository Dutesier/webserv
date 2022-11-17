#include "ConfigSetup.hpp"
#include <ostream>

ConfigSetup::ConfigSetup(int argc, char *argv[]): argc(argc), argv(argv), config(NULL) {
}

ConfigSetup::~ConfigSetup(){        
    // if (config != NULL)
    //     delete config;
}

Config* ConfigSetup::get_config() {
    Config* config = NULL;

    if (argc < 2) {
        config = config_from_default();
    } else {
        config = config_from_argument(argv[1]);
    }
    return config;
}


Config* ConfigSetup::config_from_argument(std::string argument) {
    std::ifstream configFile(argument);
    Config* config = NULL;

    if (configFile.good()) {
        // Load file contents to config struct
        config = get_config_from_file(configFile);
    } else {
        LOG_E( argument + " couldn't be opened for reading. Check if the file exists and that it has the right permissions.");
    }
    return config;
}

Config* ConfigSetup::config_from_default(){
    Config* config = NULL;
    LOG_I("No config file provided. Looking for file in default path...");

    std::ifstream configFile("/tmp/webserv/webserv.config");
    if (configFile.good()){
        // Load file contents to config struct
        config = get_config_from_file(configFile);
    } else {
        LOG_E("No config file at default location (/tmp/webserv/webserv.config). Please provide one.");
    }
    return config;
}

Config* ConfigSetup::get_config_from_file(std::ifstream& configFile) {
    ConfigParser    parser;

    parser.parse(configFile);
    Config* config = parser.get_config();
    if (parser.error()) {
        LOG_E("! CONFIG FILE ERROR !:" + parser.error()->generate_error_str());
    }
    configFile.close();
    return config;
}

