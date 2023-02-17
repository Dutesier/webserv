#ifndef _CGIHANDLER_HPP_
# define _CGIHANDLER_HPP_

# include <http/HTTPRequest.hpp>
# include <http/HTTPResponse.hpp>

# include <utils/smt.hpp>
# include <utils/Logger.hpp>

# include <cgi/CGIContext.hpp>

# include <vector>
# include <iostream>
# include <cstring>
# include <fstream>
# include <unistd.h>

namespace cgi {

enum CGI_TYPE {
    UNDEFINED,
    PYTHON,
    PHP
};


class CGIHandler {
public:
    CGIHandler(CGI_TYPE type, std::string scriptDirectory);
    ~CGIHandler();

    smt::shared_ptr<webserv::HTTPResponse> run(smt::shared_ptr<HTTPRequest>& request, int writeToFD);

    bool isValid();

private:
    CGI_TYPE    m_type;
    std::string m_directory;

    bool runAsChildProcess(int fd, smt::shared_ptr<CGIContext>& context);
    bool fileExists(std::string pathInDir);
    bool isCrawler(std::string pathInDir);

    CGIContext createContext(smt::shared_ptr<HTTPRequest>& request);

};

} // namespace cgi




#endif