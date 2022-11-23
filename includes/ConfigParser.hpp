#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "Config.hpp"
#include "Fail.hpp"
#include "Parser.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class ConfigParser : public Parser {

    public:

        /* Constructors and Destructors */
        ConfigParser(void);
        ~ConfigParser(void);

        /* Getter and Setters */
        Config* get_config(void) const;

        /* Other Functions */
        void parse(std::ifstream& file);

        /* Handlers */
        bool listen_handler(std::vector<std::string> commands);
        bool root_handler(std::vector<std::string> commands);
        bool server_handler(std::vector<std::string> commands);
        bool end_block_handler(std::vector<std::string> commands);

    private:

        /* Private Functions */
        bool valid_end(std::vector<std::string>* commands) const;
        bool is_port(std::string port) const;

        /* Private Attribute */
        Config* config;
};

#endif /* CONFIG_PARSER_HPP */
