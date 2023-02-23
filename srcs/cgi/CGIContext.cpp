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

CGIContext::CGIContext(std::vector<std::string> queryValues, std::string scriptPath){
    path = scriptPath;
    envp = queryValues;
    c_envp = NULL;

    if (!queryValues.empty()) {
        c_envp = new char*[queryValues.size() + 1];
        int envpIndex = 0;
        for (std::vector<std::string>::iterator it = envp.begin(); it != envp.end(); ++it) {
            c_envp[envpIndex] = new char[it->size() + 1];
            cgi::strcpy(c_envp[envpIndex++], it->c_str()); 
        }
        c_envp[envpIndex] = NULL;
    } else {
        c_envp = new char*[1];
        c_envp[0] = NULL;
    }

    c_argv = new char*[2];
    c_argv[0] = new char[path.size() + 1];
    cgi::strcpy(c_argv[0], path.c_str());
    c_argv[1] = NULL;

    c_path = new char[path.size() + 1];
    cgi::strcpy(c_path, path.c_str());
    LOG_D("Created context");
}

CGIContext::~CGIContext(){
    LOG_D("Deleting context");
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
