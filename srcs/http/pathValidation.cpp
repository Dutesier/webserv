#include "http/pathValidation.hpp"
#include <vector>
#include <cstring>
#include <fstream>
#include "utils/Logger.hpp"

namespace webserv::path {

bool isCrawler(std::string& pathInDir){
    int                      dir_count = 0;
    std::string              previous_dir = "..";
    std::string              current_dir = ".";
    std::vector<std::string> dirs_in_path;

    // splitting line into vector of strings
    char* word = strtok(const_cast<char*>(pathInDir.c_str()), "/");
    while (word) {
        dirs_in_path.push_back(word);
        word = strtok(NULL, "/");
        LOG_D("word: " + dirs_in_path.back());
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

bool fileExists(std::string& directory, std::string& pathInDir){
    std::string filepath = directory + pathInDir;
    LOG_D("Checking if file exists " + filepath);
    std::ifstream f(
        filepath
            .c_str()); // Lets make sure that we dont have /cgi/python//fu.py

    LOG_D("File exists: " + filepath);
    return f.good();
}

std::string formattedFullPath(std::string& directory, std::string& pathInDir) {
    if (*(directory.rbegin()) == '/') {
        if (*(pathInDir.begin()) == '/') {
            return directory.substr(0, directory.size() - 1) + pathInDir;
        }
    }
    return directory + pathInDir;
}


} // namespace webserv::path