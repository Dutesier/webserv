#include <cgi/CGIContext.hpp>

namespace cgi {

void strcpy(char* me, char const* other) {
    if (!other || !me) {
        LOG_F("Failed copy");
        return;
    }

    int i;
    for (i = 0; other[i] != '\0'; i++) { me[i] = other[i]; }
    me[i] = '\0';
}

void CGIContext::fill_envp(std::string name, std::string val) {
    if (!name.empty() && !val.empty()) { envp.push_back(name + "=" + val); }
}

CGIContext::CGIContext(smt::shared_ptr<HTTPRequest> request) {

    Uri	uri = Uri(request->getResource());
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
    fill_envp("PATH_INFO", uri.getPathInfo());
    fill_envp("SCRIPT_NAME", uri.getScriptName());
    fill_envp("QUERY_STRING", uri.m_query);
    fill_envp("USER_AGENT", request->getHeader("User-Agent"));
    fill_envp("PATH_TRANSLATED",
              std::string(uri.getPathInfo()));
    fill_envp("DOCUMENT_ROOT", directory);
    fill_envp("SERVER_PORT", uri.m_port);

    c_envp = new char*[envp.size() + 1];
    int envpIndex = 0;
    for (std::vector<std::string>::iterator it = envp.begin(); it != envp.end();
         ++it) {
        c_envp[envpIndex] = new char[it->size() + 1];
        cgi::strcpy(c_envp[envpIndex++], it->c_str());
    }
    c_envp[envpIndex] = NULL;

    c_argv = new char*[2];
    std::string path =
        directory.substr(0, directory.size()) + uri.getScriptName();
    c_argv[0] = new char[path.size() + 1];
    cgi::strcpy(c_argv[0], path.c_str());
    c_argv[1] = NULL;

    c_path = new char[path.size() + 1];
    cgi::strcpy(c_path, path.c_str());
}

CGIContext::~CGIContext() {
    int envpIndex = 0;
    if (c_envp) {
        for (std::vector<std::string>::iterator it = envp.begin();
             it != envp.end(); ++it) {
            delete c_envp[envpIndex++];
        }
        delete[] c_envp;
    }

    // Argv is a null terminated Char**
    if (c_argv) {
        delete c_argv[0];
        delete[] c_argv;
    }

    if (c_path) { delete c_path; }
}

char* CGIContext::getPath() const { return c_path; }

char** CGIContext::getEnvp() const { return c_envp; }

char** CGIContext::getArgv() const { return c_argv; }

std::vector<std::string>	CGIContext::getVectorEnvp() const { return envp; }


} // namespace cgi

std::ostream& operator<<(std::ostream& os, cgi::CGIContext const& ctx) {
    char**      argv = ctx.getArgv();
    char**      envp = ctx.getEnvp();
    char const* path = ctx.getPath();

    if (argv != NULL) {
        os << "Printing argv." << std::endl;
        for (int i = 0; argv[i] != NULL; ++i) { os << argv[i] << std::endl; }
    }
    if (envp != NULL) {
        os << "Printing envp." << std::endl;
        for (int i = 0; envp[i] != NULL; ++i) { os << envp[i] << std::endl; }
    }
    if (path != NULL) { os << "Path: " << path << std::endl; }
    return os;
}
