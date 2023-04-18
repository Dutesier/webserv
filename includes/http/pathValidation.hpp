#pragma once

#include <string>
#include <cstdlib>

namespace webserv::path {

bool isCrawler(std::string& pathInDir);
bool fileExists(std::string& directory, std::string& pathInDir);
std::string formattedFullPath(std::string& directory, std::string& pathInDir);


} // namespace webserv::path