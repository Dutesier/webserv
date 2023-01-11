#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "server/ServerConfig.hpp"
#include "utils/Error.hpp"
#include "utils/Logger.hpp"
#include "utils/smt.hpp"

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
 ERROR_(M + ": " + L, webserv::Error::invalid_syntax);                         \
 m_file.close();                                                               \
 throw(InvalidSyntaxException());

class Config {

    public:

        Config(int argc, char* argv[]);
        ~Config(void);

        std::vector<smt::shared_ptr<ServerConfig> > server_config(void);

#ifndef GTEST_TESTING

    private:

#endif

        struct impl;
        smt::shared_ptr<impl> m_impl;
};

struct Config::impl {

        impl(int argc, char* argv[]);
        ~impl(void);

        struct InvalidFileException : public std::exception {
                char const* what(void) const throw();
        };

        struct InvalidSyntaxException : public std::exception {
                char const* what(void) const throw();
        };

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

        static std::string const d_path;
        static std::string const d_file;
        static std::string const d_method;

        std::ifstream                               m_file;
        std::vector<smt::shared_ptr<ServerConfig> > m_server;
};

} // namespace webserv

#endif /* CONFIG_HPP */
