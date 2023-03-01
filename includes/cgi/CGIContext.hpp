#ifndef _CGICONTEXT_HPP_
# define _CGICONTEXT_HPP_

# include <iostream>
# include <vector>
# include <map>

# include <utils/smt.hpp>
# include <utils/Logger.hpp>
# include <http/HTTPRequest.hpp>

namespace cgi
{
class CGIContext {
public:
    // CGIContext(std::vector<std::string> queryValues, std::string scriptPath);
    CGIContext(smt::shared_ptr<HTTPRequest> request, std::string root_directory);
    ~CGIContext();

    void    fill_envp(std::string name, std::string val);
    char*     getPath() const;
    char**          getEnvp() const;
    char**          getArgv() const;
private:
    std::string                         directory;
    std::vector<std::string>            envp;


    char**  c_envp;
    char**  c_argv;
    char*   c_path;
};
} // namespace cgi

std::ostream& operator<<(std::ostream& os, cgi::CGIContext const& ctx);

#endif