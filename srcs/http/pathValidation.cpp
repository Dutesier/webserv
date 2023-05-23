#include "http/pathValidation.hpp"

#include "utils/Logger.hpp"

#include <cstring>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

namespace webserv::path {

bool isCrawler(std::string& pathInDir) {
    int                      dir_count = 0;
    std::string              previous_dir = "..";
    std::string              current_dir = ".";
    std::vector<std::string> dirs_in_path;

    // splitting line into vector of strings
    char* word = strtok(const_cast<char*>(pathInDir.c_str()), "/");
    while (word) {
        dirs_in_path.push_back(word);
        word = strtok(NULL, "/");
    }

    for (std::vector<std::string>::iterator it = dirs_in_path.begin();
         it != dirs_in_path.end(); ++it) {
        if (*it == previous_dir) { dir_count--; }
        else if (*it != current_dir) { dir_count++; }

        // If at any point we try to go back further than what we are allowed
        if (dir_count < 0) {
            LOG_W("Invalid Access Atempt");
            return true;
        }
    }
    return false;
}

bool fileExists(std::string& formattedFullPath) {
    std::ifstream f(
        formattedFullPath
            .c_str()); // Lets make sure that we dont have /cgi/python//fu.py

    return f.good();
}

bool isDirectory(std::string& formattedFullPath) {
    struct stat s;
    if (stat(formattedFullPath.c_str(), &s) == 0) {
        if (s.st_mode & S_IFDIR) { return true; }
    }
    return false;
}

std::string formattedFullPath(std::string& directory, std::string& pathInDir) {
    // If directory does not end with / and pathInDir does not start with /, we
    // want to add /
    if (*(directory.rbegin()) != '/') {
        if (*(pathInDir.begin()) != '/') { return directory + "/" + pathInDir; }
    }

    // If directory ends with / and pathInDir starts with /, we dont want to
    // duplicate /
    if (*(directory.rbegin()) == '/') {
        if (*(pathInDir.begin()) == '/') {
            return directory.substr(0, directory.size() - 1) + pathInDir;
        }
    }
    return directory + pathInDir;
}

} // namespace webserv::path