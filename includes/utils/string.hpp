#ifndef STRING_HPP
#define STRING_HPP

#include <cstring>
#include <string>
#include <vector>

namespace ft {

class string : public std::string {
    public:

        string(void);
        string(char const* s);
        string(std::string const& s);

        std::vector<string> split(string const& delimiter) const;
        string&             trim(void);

        bool isnumeric(void) const;
};

} // namespace ft

#endif /* STRING_HPP */