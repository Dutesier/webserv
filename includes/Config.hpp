#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Error.hpp"
#include "Logger.hpp"
#include "ServerConfig.hpp"

#include <cstring>
#include <map>
#include <sstream>
#include <vector>

namespace webserv {

#define ERROR_(M, T)                                                           \
 webserv::Error err(M, T);                                                     \
 FLOG_E(err.message());

#define error_file(M)                                                          \
 ERROR_(M, webserv::Error::invalid_file);                                      \
 throw(InvalidFileException());

#define error_syntax(M, L)                                                     \
 for (std::vector<ServerConfig*>::iterator it = this->server.begin();          \
      it != this->server.end(); ++it)                                          \
  delete *it;                                                                  \
 ERROR_(M + ": " + L, webserv::Error::invalid_syntax);                         \
 throw(InvalidSyntaxException());

class Config {

    public:

        /* Constructor and Destructor */
        Config(int argc, char* argv[]);
        ~Config(void);

        /* Getters and Setters */
        std::vector<ServerConfig*> server_config(void) const;

#ifndef GTEST_TESTING

    private:

#endif

        /* PImpl Object */
        struct impl;
        impl* m_impl;
};

struct Config::impl {

        /* Constructor and Destructor */
        impl(int argc, char* argv[]);
        ~impl(void);

        /* Exceptions */
        struct InvalidFileException : public std::exception {
                char const* what(void) const throw();
        };

        struct InvalidSyntaxException : public std::exception {
                char const* what(void) const throw();
        };

        /* Other Functions */
        bool server_cmd(std::string line);
        bool location_cmd(std::string line);
        bool cmd_listen(std::vector<std::string> cmd);
        bool cmd_server_name(std::vector<std::string> cmd);
        bool cmd_error_page(std::vector<std::string> cmd);
        bool cmd_max_size(std::vector<std::string> cmd);
        bool cmd_root(std::vector<std::string> cmd);
        bool cmd_autoindex(std::vector<std::string> cmd);
        bool cmd_index(std::vector<std::string> cmd);
        bool cmd_location(std::vector<std::string> cmd);
        bool cmd_lroot(std::vector<std::string> cmd);
        bool cmd_fastcgi_pass(std::vector<std::string> cmd);
        bool cmd_request_method(std::vector<std::string> cmd);

        /* Static Members */
        static std::string const                d_path;
        static std::string const                d_file;
        static std::map<int, std::string> const exit_code;
        static std::string const                method;

        static std::map<int, std::string> init_exit_code(void);

        /* Member Attributes */
        std::ifstream              file;
        std::vector<ServerConfig*> server;
};

} // namespace webserv

#endif /* CONFIG_HPP */
