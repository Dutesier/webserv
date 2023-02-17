#ifndef _CGICONTEXT_HPP_
# define _CGICONTEXT_HPP_

# include <iostream>
# include <vector>
# include <map>

# include <utils/smt.hpp>
# include <http/HTTPRequest.hpp>

namespace cgi
{
class CGIContext {
public:
    CGIContext(std::vector<std::string> queryValues, std::string scriptPath);
    ~CGIContext();

    const char*     getPath();
    char**          getEnvp();
    char**          getArgv();
private:
    std::string                         path;
    std::vector<std::string>            envp;

    char** c_envp;
    char** c_argv;
};
} // namespace cgi


#endif