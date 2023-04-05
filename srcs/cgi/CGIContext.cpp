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

void CGIContext::fillEnvp(std::string name, std::string val) {
    if (!name.empty() && !val.empty()) { envp.push_back(name + "=" + val); }
}

CGIContext::CGIContext(smt::shared_ptr<webserv::HTTPRequest> request,
                       std::string                           rootDirectory)
    : directory(rootDirectory) {
    std::stringstream ss;
    m_envp = NULL;

    ss << request->getContent().length();
    fillEnvp("CONTENT_LENGTH", ss.str());
    fillEnvp("CONTENT_TYPE", request->getHeader("Content-Type"));
    if (request->getMethod() == webserv::HTTPRequest::GET)
        fillEnvp("REQUEST_METHOD", "GET");
    else if (request->getMethod() == webserv::HTTPRequest::POST)
        fillEnvp("REQUEST_METHOD", "POST");
    fillEnvp("SERVER_PROTOCOL", "HTTP/1.1");
    fillEnvp("PATH_INFO", request->getPathInfo());
    fillEnvp("SCRIPT_NAME", request->getScriptName());
    fillEnvp("QUERY_STRING", request->getQueriesFromResource());
    fillEnvp("USER_AGENT", request->getHeader("User-Agent"));
    fillEnvp("PATH_TRANSLATED",
              std::string(directory + request->getScriptName()));
    fillEnvp("DOCUMENT_ROOT", directory);
    // ss.str(std::string());
    // ss << port; Need a way to get server port
    // fillEnvp("SERVER_PORT", ss.str());

    m_envp = new char*[envp.size() + 1];
    int envpIndex = 0;
    for (std::vector<std::string>::iterator it = envp.begin(); it != envp.end();
         ++it) {
        m_envp[envpIndex] = new char[it->size() + 1];
        cgi::strcpy(m_envp[envpIndex++], it->c_str());
    }
    m_envp[envpIndex] = NULL;

    m_argv = new char*[2];
    std::string path =
        directory.substr(0, directory.size()) + request->getScriptName();
    m_argv[0] = new char[path.size() + 1];
    cgi::strcpy(m_argv[0], path.c_str());
    m_argv[1] = NULL;

    m_path = new char[path.size() + 1];
    cgi::strcpy(m_path, path.c_str());
}

CGIContext::~CGIContext() {
    int envpIndex = 0;
    if (m_envp) {
        for (std::vector<std::string>::iterator it = envp.begin();
             it != envp.end(); ++it) {
            delete m_envp[envpIndex++];
        }
        delete[] m_envp;
    }

    // Argv is a null terminated Char**
    if (m_argv) {
        delete m_argv[0];
        delete[] m_argv;
    }

    if (m_path) { delete m_path; }
}

char* CGIContext::getPath() const { return m_path; }

char** CGIContext::getEnvp() const { return m_envp; }

char** CGIContext::getArgv() const { return m_argv; }

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
