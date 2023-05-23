#pragma once

#include <cstdlib>
#include <string>

namespace webserv::path {

bool        isCrawler(std::string& pathInDir);
bool        fileExists(std::string& formattedFullPath);
bool        isDirectory(std::string& formattedFullPath);
std::string formattedFullPath(std::string& directory, std::string& pathInDir);

} // namespace webserv::path