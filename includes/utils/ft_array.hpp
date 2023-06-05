#ifndef FT_ARRAY_HPP
#define FT_ARRAY_HPP

#include <cstring>
#include <string>
#include <vector>

namespace ft {

class array {
    public:

        array(void);
        array(std::vector<std::string> const& argc);
        array(array const& src);

        ~array(void);

        array& operator=(array const& rhs);

        operator char**(void) const;

        static void erase(char** arr);

    private:

        std::vector<std::string> m_argc;
};

} // namespace ft

#endif /* FT_ARRAY_HPP */