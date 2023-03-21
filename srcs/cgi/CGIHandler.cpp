#include "cgi/CGIHandler.hpp"
#include <sys/wait.h>
#include <cstdlib>

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
    if (isCrawler(request->getRefinedResource()) || !fileExists(request->getRefinedResource())) {
        LOG_W("CGI wont run - Invalid permissions for " + request->getRefinedResource());
        return NULL;
    }

    smt::shared_ptr<CGIContext> ctx = smt::make_shared(new CGIContext(request, m_directory));
    return (runAsChildProcess(writeToFD, ctx, request->getContent()));
}

smt::shared_ptr<webserv::HTTPResponse> CGIHandler::runAsChildProcess(int fd, smt::shared_ptr<CGIContext>& context, std::string req_content){
    
    LOG_D("running child process");
    FILE* input = tmpfile();
    int input_fd = fileno(input);

    FILE* output = tmpfile();
    int output_fd = fileno(output);

    int stdin_reference = dup(STDIN_FILENO);
    int stdout_reference = dup(STDOUT_FILENO);

    if (!input || !output || stdin_reference < 0)
        LOG_E("Failed to create tmp files");
    if (input_fd < 0)
        LOG_E("Failed to get temporary infile fd");
    write(input_fd, req_content.c_str(), req_content.size());
    rewind(input);
    pid_t pid = fork();
    if (pid < 0) {
        LOG_E("Failed to spawn child process");
    } else if (pid == 0){
        // Direct I/O to temporary file;
        // std::cout << context << std::endl;
        dup2(input_fd, STDIN_FILENO);
		dup2(output_fd, STDOUT_FILENO);
        execve(context->getPath(), context->getArgv(), context->getEnvp());
        perror("execve() failed");
        // Child process
        LOG_E("Failed call to execve in child process");
        exit(500);
    }
    // From here on out we are always in parent cause child either executed or exited
    int status;
    smt::shared_ptr<webserv::HTTPResponse> resp;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) {
        resp = smt::make_shared(new webserv::HTTPResponse(500, std::map<std::string, std::string>()));
    } else {
        int bytesRead = 0;
        char buf[2049];
        lseek(output_fd, 0, SEEK_SET);
        while ((bytesRead = read(output_fd, buf, 2048)) > 0) {
            buf[bytesRead] = '\0';
        }
        std::string body(buf);
        std::map<std::string, std::string> headers;
        // headers["Content-Length:"] = std::to_string(body.size());
        resp = smt::make_shared<webserv::HTTPResponse>(new webserv::HTTPResponse(200, headers, body));
    }

    // Reset STDIN and STDOUT
    dup2(stdin_reference, STDIN_FILENO);
	dup2(stdout_reference, STDOUT_FILENO);

    // Close temporary files
	fclose(input);
	fclose(output);

    
    return resp;
}

void CGIHandler::updateScriptDirectory(std::string dir) {
    m_directory = dir;
}

bool CGIHandler::fileExists(std::string pathInDir){
    std::string filepath = m_directory + pathInDir;
    LOG_D("Checking if file exists " + filepath);
    std::ifstream f(filepath.c_str()); // Lets make sure that we dont have /cgi/python//fu.py
    
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
