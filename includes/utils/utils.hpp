#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <string>
#include <vector>

namespace webserv {

class utils {

public:

	static std::string                d_file(std::string d = "");
	static std::string                d_path(std::string d = "");
	static std::string                d_root(std::string d = "");
	static std::vector<std::string>   index_pages(void);
	static std::vector<std::string>   allowed_methods(void);
	static std::map<int, std::string> status_codes(void);

};

} // namespace utils

#endif /* UTILS_HPP */
