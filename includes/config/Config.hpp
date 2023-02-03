#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "config/Blocks.hpp"
#include "utils/Logger.hpp"

#include <fstream>
#include <sstream>
#include <vector>

namespace webserv {

#define ERROR_FILE(filename)                                                   \
 LOG_E(filename + ": failed to open");                                         \
 throw(InvalidFileException())

#define ERROR_SYNTAX(MSG)                                                      \
 std::stringstream ss;                                                         \
 ss << i;                                                                      \
 LOG_E(m_filename + ":" + ss.str() + ": " + line +                             \
       "\n(syntax error): " + MSG);                                            \
 throw(InvalidSyntaxException())

class Config {

    public:

        Config(int argc, char* argv[]);
        ~Config(void);

        std::vector<std::string> split_line(std::string line);

        std::vector< smt::shared_ptr<ServerBlock> > blocks(void);

        struct InvalidFileException : public std::exception {
                char const* what(void) const throw();
        };

        struct InvalidSyntaxException : public std::exception {
                char const* what(void) const throw();
        };

#ifndef GTEST_TESTING

    private:

#endif /* GTEST_TESTING */

        std::ifstream                               m_file;
        std::string                                 m_filename;
        std::vector< smt::shared_ptr<ServerBlock> > m_blocks;
};

} // namespace webserv

#endif /* CONFIG_HPP */
