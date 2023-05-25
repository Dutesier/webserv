#ifndef URI_HPP
#define URI_HPP

#include "utils/Logger.hpp"
#include "utils/string.hpp"

#include <string>
#include <vector>

class Uri {

    public:

        Uri(ft::string uri);
        ~Uri(void);

        void route(std::string target, std::string root);
        std::string getPathInfo(); // Get the extra info after the script name
        std::string getScriptName(); // Get the name of the script


        // std::string m_target;
        std::string m_scheme;
        std::string m_port;
        std::string m_host;
        std::string m_path;
        std::string m_routedPath;
        std::string m_query;
};

#endif /* URI_HPP */