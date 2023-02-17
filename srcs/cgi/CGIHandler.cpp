#include "cgi/CGIHandler.hpp"

namespace cgi {

CGIHandler::CGIHandler(CGI_TYPE type, std::string scriptDirectory)
    : m_type(type)
    , m_directory(scriptDirectory) {
}

CGIHandler::~CGIHandler(){

}

bool CGIHandler::isValid(){
    return m_type != UNDEFINED;
}

smt::shared_ptr<webserv::HTTPResponse> CGIHandler::run(smt::shared_ptr<HTTPRequest>& request, int writeToFD){
    if (isCrawler(request->getRefinedResource()) || !fileExists(request->getRefinedResource()))
        return NULL;

    smt::shared_ptr<CGIContext> ctx = smt::make_shared(new CGIContext(request->getQueriesFromResource(), std::string(m_directory + request->getRefinedResource())));
    bool success = runAsChildProcess(writeToFD, ctx);
    if (success) {
        return (smt::make_shared(new webserv::HTTPResponse(200, std::map<std::string, std::string>())));
    }

    return NULL;
}

bool CGIHandler::runAsChildProcess(int fd, smt::shared_ptr<CGIContext>& context){
    
    pid_t pid = fork();
    if (pid < 0) {
        LOG_E("Failed to spawn child process");
    } else if (pid > 0){
        // Parent process
    } else {
        // Direct output from STDOUT to connection
        dup2(fd, STDOUT_FILENO);
        execve(context->getPath(), context->getArgv(), context->getEnvp());
        perror("execve() failed");
        // Child process
        LOG_E("Failed call to execve in child process");
        return false;
    }
    return true;
}

bool CGIHandler::fileExists(std::string pathInDir){
    std::ifstream f((m_directory + pathInDir).c_str()); // Lets make sure that we dont have /cgi/python//fu.py
    return f.good();
}

bool CGIHandler::isCrawler(std::string pathInDir){
    int                         dir_count = 0;
    std::string                 previous_dir = "..";
    std::string                 current_dir = ".";
    std::vector<std::string>    dirs_in_path;

    // splitting line into vector of strings
    char* word = strtok(const_cast<char*>(pathInDir.c_str()), "/");
    while (word) {
        dirs_in_path.push_back(word);
        word = strtok(NULL, "/");
    }

    for (std::vector<std::string>::iterator it = dirs_in_path.begin(); it != dirs_in_path.end(); ++it) {
        if (*it == previous_dir) {
            dir_count--;
        } else if (*it != current_dir) {
            dir_count++;
        }

        // If at any point we try to go back further than what we are allowed
        if (dir_count < 0){
            LOG_W("Invalid Access Atempt");
            return true;
        }
    }
    return false;
}


}; // namespace cgi
