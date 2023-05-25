#ifndef _CGICONTEXT_HPP_
#define _CGICONTEXT_HPP_

#include <map>
#include <vector>
#include <iostream>

#include "http/Uri.hpp" 
#include "utils/smt.hpp"
#include "utils/Logger.hpp"
#include "http/HTTPRequest.hpp"

namespace cgi {
class CGIContext {
    public:

        // CGIContext(std::vector<std::string> queryValues, std::string
        // scriptPath);
        CGIContext(smt::shared_ptr<HTTPRequest> request);
        ~CGIContext();

        void   fill_envp(std::string name, std::string val);
        char*  getPath() const;
        char** getEnvp() const;
        char** getArgv() const;
		std::vector<std::string> getVectorEnvp() const;

    private:

        std::string              directory;
        std::vector<std::string> envp;

        char**	c_envp;
        char**	c_argv;
        char* 	c_path;
};
} // namespace cgi

std::ostream& operator<<(std::ostream& os, cgi::CGIContext const& ctx);

#endif