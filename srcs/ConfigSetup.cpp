#include "ConfigSetup.hpp"

ConfigSetup::ConfigSetup(int argc, char *argv[]) {
    if (argc < 2) {
        config = config_from_default();
    } else {
        config = config_from_argument(argv[1]);
    }
}

ConfigSetup::~ConfigSetup(){        
    if (config != NULL)
        delete config;
    }

Config* ConfigSetup::get_config() {
    return config;
}


Config* ConfigSetup::config_from_argument(std::string argument) {
    std::ifstream configFile(argument);
    if (configFile.good()) {
        // Load file contents to config struct
        config = get_config_from_file(configFile);
    } else {
        LOG_E( argument + " couldn't be opened for reading. Check if the file exists and that it has the right permissions");
        if (config != NULL)
            delete config;
        config = NULL;
    }
    return config;
}

Config* ConfigSetup::config_from_default(){
    LOG_I("No config file provided. Looking for file in default path");

    std::ifstream configFile("/tmp/webserv/webserv.config");
    if (configFile.good()){
        // Load file contents to config struct
        config = get_config_from_file(configFile);
    } else {
        LOG_E("No config file at default location (/tmp/webserv/webserv.config). Please provide one");
        if (config != NULL)
            delete config;
        config = NULL;
    }
    return config;
}

Config* ConfigSetup::get_config_from_file(std::ifstream& configFile) {
    // Parser goes here
    configFile.close();
    return new Config;
}

