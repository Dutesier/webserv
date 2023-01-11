#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "server/Blocks.hpp"
#include "utils/Logger.hpp"
#include "utils/smt.hpp"
#include "utils/utils.hpp"

#include <cstring>
#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <sstream>
#include <vector>

namespace webserv {

class Config {

    public:

        Config(int argc, char* argv[]);
        ~Config(void);

        std::vector<smt::shared_ptr<ServerBlock> > config(void);

#ifndef GTEST_TESTING

    private:

#endif

        struct impl;
        smt::shared_ptr<impl> m_impl;
};

struct Config::impl {

        typedef std::pair<bool, std::string> result_type;

        impl(int argc, char* argv[]);
        ~impl(void);

        struct InvalidFileException : public std::exception {
                char const* what(void) const throw();
        };

        struct InvalidSyntaxException : public std::exception {
                char const* what(void) const throw();
        };

        std::ifstream                              m_file;
        std::vector<smt::shared_ptr<ServerBlock> > m_config;

        std::string parse(std::string filename);

        void        error_file(std::string filename) const;
        std::string generate_error(std::string filename, std::string line,
                                   int nu, std::string msg) const;
        void        error_syntax(std::string msg) const;

        result_type server(std::string line);
        result_type location(std::string line);

        result_type cmd_listen(std::vector<std::string> cmd);
        result_type cmd_server_name(std::vector<std::string> cmd);
        result_type cmd_error_page(std::vector<std::string> cmd);
        result_type cmd_max_size(std::vector<std::string> cmd);
        result_type cmd_root(std::vector<std::string> cmd);
        result_type cmd_autoindex(std::vector<std::string> cmd);
        result_type cmd_index(std::vector<std::string> cmd);
        result_type cmd_lroot(std::vector<std::string> cmd);
        result_type cmd_fastcgi_pass(std::vector<std::string> cmd);
        result_type cmd_request_method(std::vector<std::string> cmd);
};

} // namespace webserv

#endif /* CONFIG_HPP */
