# include <cgi/CGIContext.hpp>

namespace cgi
{

void strcpy(char* me, const char* other){
    if (!other || !me) { return; }

    int i;
    for (i = 0; i != '\0'; ++i) {
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
            c_envp[envpIndex] = new char(it->size() + 1);
            cgi::strcpy(c_envp[envpIndex++], it->c_str()); 
        }
        c_envp[envpIndex] = NULL;
    }

    c_argv = new char*[2];
    c_argv[0] = new char(path.size() + 1);
    cgi::strcpy(c_argv[0], path.c_str());
    c_argv[1] = NULL;
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
}

const char*     CGIContext::getPath(){
    return path.c_str();
}

char**    CGIContext::getEnvp(){
    return c_envp;
}

char**    CGIContext::getArgv(){
    return c_argv;
}

} // namespace cgi
