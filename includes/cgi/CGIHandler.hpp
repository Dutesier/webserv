#ifndef _CGIHANDLER_HPP_
#define _CGIHANDLER_HPP_

#include <cgi/CGIContext.hpp>
#include <cstring>
#include <fstream>
#include <http/HTTPRequest.hpp>
#include <http/HTTPResponse.hpp>
#include <iostream>
#include <unistd.h>
#include <utils/Logger.hpp>
#include <utils/smt.hpp>
#include <vector>

namespace cgi {

enum CGI_TYPE { UNDEFINED, PYTHON, PHP };

class CGIHandler {
    public:

        CGIHandler(CGI_TYPE type, std::string scriptDirectory);
        ~CGIHandler();

        smt::shared_ptr<webserv::HTTPResponse>
            run(smt::shared_ptr<webserv::HTTPRequest>& request, int writeToFD);

        bool isValid();
        void updateScriptDirectory(std::string dir);

    private:

        CGI_TYPE    m_type;
        std::string m_directory;

        smt::shared_ptr<webserv::HTTPResponse>
             runAsChildProcess(int fd, smt::shared_ptr<CGIContext>& context,
                               std::string req_content);
        bool fileExists(std::string pathInDir);
        bool isCrawler(std::string pathInDir);

        CGIContext
            createContext(smt::shared_ptr<webserv::HTTPRequest>& request);
};

} // namespace cgi

#endif
