#ifndef _CGICONTEXT_HPP_
#define _CGICONTEXT_HPP_

#include <http/HTTPRequest.hpp>
#include <iostream>
#include <map>
#include <utils/Logger.hpp>
#include <utils/smt.hpp>
#include <vector>

namespace cgi {
class CGIContext {
    public:

        // CGIContext(std::vector<std::string> queryValues, std::string
        // scriptPath);
        CGIContext(smt::shared_ptr<webserv::HTTPRequest> request,
                   std::string                           rootDirectory);
        ~CGIContext(void);

        void   fillEnvp(std::string name, std::string val);
        char*  getPath(void) const;
        char** getEnvp(void) const;
        char** getArgv(void) const;

    private:

        std::string              directory;
        std::vector<std::string> envp;

        char** m_envp;
        char** m_argv;
        char*  m_path;
};

} // namespace cgi

std::ostream& operator<<(std::ostream& os, cgi::CGIContext const& ctx);

#endif