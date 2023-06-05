#ifndef CGI_HPP
#define CGI_HPP

#include "config/Options.hpp"
#include "http/Request.hpp"
#include "utils/smt.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace cgi {

class CgiHandler {
    public:

        CgiHandler(void);
        CgiHandler(char* path, char** argv, char** envp,
                   std::string const& body = "");
        CgiHandler(CgiHandler const& src);

        ~CgiHandler(void);

        CgiHandler& operator=(CgiHandler const& rhs);

        std::string run(void) const;

    private:

        std::string runAsChildProcess(void) const;
        std::string get(std::string key) const;

        char*  m_path;
        char** m_argv;
        char** m_envp;

        std::string m_body;
};

enum CgiType { Py, Php, Cgi, Unknown };

CgiType convertCgiExtension(std::string const& cgiExtension);

std::vector<std::string> splitInfoFromPath(std::string const& path);

std::string runCgiScript(smt::shared_ptr<http::Request> const request,
                         smt::shared_ptr<config::Opts> const  opts);

} // namespace cgi

#endif /* CGI_HPP */