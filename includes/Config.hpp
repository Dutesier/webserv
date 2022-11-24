#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Error.hpp"
#include "Logger.hpp"

#include <cstring>
#include <exception>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace webserv {

#define ERROR_(M, T)                                                           \
 webserv::Error err(M, T);                                                     \
 FLOG_E(err.message());

#define error_file(M)                                                          \
 ERROR_(M, webserv::Error::invalid_file);                                      \
 throw(InvalidFileException());

#define error_syntax(M, L)                                                     \
 ERROR_(M + ": " + L, webserv::Error::invalid_syntax);                         \
 throw(InvalidSyntaxException());

class Config {

    public:

        /* Constructor and Destructor */
        Config(int argc, char* argv[]);
        ~Config(void);

        /* Nested Classes */
        class LocationBlock {
            public:

                /* Constructor and Destructor */
                LocationBlock(std::string line);
                ~LocationBlock(void);

                /* Other Functions */
                bool add_directive(std::string line);
                bool directive_root(std::vector<std::string> command);
                bool directive_fastcgi_pass(std::vector<std::string> command);
                bool directive_request_method(std::vector<std::string> command);

                /* Static Attributes */
                static std::string methods;

                /* Attributes */
                std::string              uri;
                std::string              root;
                std::string              fastcgi_pass;
                std::vector<std::string> request_method;
        };

        class ServerBlock {
            public:

                /* Constructor and Destructor */
                ServerBlock(void);
                ~ServerBlock(void);

                /* Other Functions */
                bool add_directive(std::string line);
                bool directive_listen(std::vector<std::string> command);
                bool directive_server_name(std::vector<std::string> command);
                bool directive_error_page(std::vector<std::string> command);
                bool directive_max_size(std::vector<std::string> command);
                bool directive_access_log(std::vector<std::string> command);
                bool directive_root(std::vector<std::string> command);
                bool directive_autoindex(std::vector<std::string> command);
                bool directive_index(std::vector<std::string> command);
                bool directive_location(std::vector<std::string> command);

                /* Attributes */
                bool        autoindex;
                unsigned    body_size;
                unsigned    port;
                std::string host;
                std::string access_log;
                std::string root;

                std::vector<LocationBlock*> location_block;
                std::vector<std::string>    index;
                std::vector<std::string>    server_name;
                std::map<int, std::string>  error_page;
        };

        /* Getters and Setters */
        std::vector<ServerBlock*> server_configs(void) const;

        /* Exceptions */
        struct InvalidFileException : public std::exception {
                char const* what(void) const throw();
        };

        struct InvalidSyntaxException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        /* Private Attributes */
        std::ifstream             file;
        std::vector<ServerBlock*> server_block;

        /* Private Static Attributes */
        static std::string default_file;
        static std::string default_path;

        static std::map<int, std::string> const exit_code;
        static std::map<int, std::string>       init_exit_code(void);
};

} // namespace webserv

#endif /* CONFIG_HPP */
