# include <cgi/CGIContext.hpp>

namespace cgi
{

void strcpy(char* me, const char* other){
    if (!other || !me) { LOG_F("Failed copy"); return; }

    int i;
    for (i = 0; other[i] != '\0'; i++) {
        me[i] = other[i];
    }
    me[i] = '\0';
}

void    CGIContext::fill_envp(std::string name, std::string val) {
    if (!name.empty() && !val.empty()) {
        envp.push_back(name + "=" + val);
    }    

}

CGIContext::CGIContext(smt::shared_ptr<HTTPRequest> request, std::string root_directory): directory(root_directory) {
    std::stringstream ss;
    c_envp = NULL;
    
    ss << request->getContent().length();
    fill_envp("CONTENT_LENGTH", ss.str());
    fill_envp("CONTENT_TYPE", request->getHeader("Content-Type"));
    if (request->getMethod() == webserv::GET)
        fill_envp("REQUEST_METHOD", "GET");
    else if (request->getMethod() == webserv::POST)
        fill_envp("REQUEST_METHOD", "POST");
    fill_envp("SERVER_PROTOCOL", "HTTP/1.1");
    fill_envp("PATH_INFO", request->getPathInfo());
    fill_envp("SCRIPT_NAME", request->getScriptName());
    fill_envp("QUERY_STRING", request->getQueriesFromResource());
    fill_envp("USER_AGENT", request->getHeader("User-Agent"));
    fill_envp("PATH_TRANSLATED", std::string(directory + request->getScriptName()));
    fill_envp("DOCUMENT_ROOT", directory);
    // ss.str(std::string());
    // ss << port; Need a way to get server port
    // fill_envp("SERVER_PORT", ss.str());


        c_envp = new char*[envp.size() + 1];
        int envpIndex = 0;
        for (std::vector<std::string>::iterator it = envp.begin(); it != envp.end(); ++it) {
            c_envp[envpIndex] = new char[it->size() + 1];
            cgi::strcpy(c_envp[envpIndex++], it->c_str()); 
        }
        c_envp[envpIndex] = NULL;

    c_argv = new char*[2];
    c_argv[0] = new char[directory.size() + 1];
    cgi::strcpy(c_argv[0], directory.c_str());
    c_argv[1] = NULL;

    c_path = new char[directory.size() + 1];
    cgi::strcpy(c_path, directory.c_str());
}

CGIContext::~CGIContext(){
    int envpIndex = 0;
    if (c_envp) {
        for (std::vector<std::string>::iterator it = envp.begin(); it != envp.end(); ++it) {
            delete c_envp[envpIndex++];
        }
        delete[] c_envp;
    }

    // Argv is a null terminated Char**
    if (c_argv) {
        delete c_argv[0];
        delete[] c_argv;
    }

    if (c_path) {
        delete c_path;
    }
}

char*     CGIContext::getPath() const{
    return c_path;
}

char**    CGIContext::getEnvp() const{
    return c_envp;
}

char**    CGIContext::getArgv() const{
    return c_argv;
}


} // namespace cgi

std::ostream& operator<<(std::ostream& os, cgi::CGIContext const& ctx) {
    char** argv = ctx.getArgv();
    char** envp = ctx.getEnvp();
    const char* path = ctx.getPath();
    
    if (argv != NULL) {
        os << "Printing argv." << std::endl;
        for (int i = 0; argv[i] != NULL; ++i) {
            os << argv[i];
        }
    }
    if (envp != NULL) {
        os << "Printing envp." << std::endl;
        for (int i = 0; envp[i] != NULL; ++i) {
            os << envp[i];
        }
    }
    if (path != NULL) {
        os << "Path: " << path;
    }
    return os;
}
