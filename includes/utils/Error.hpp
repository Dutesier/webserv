#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

namespace webserv {

class Error {

    public:

        typedef enum error { invalid_file, invalid_syntax } error_t;

        Error(std::string msg, error_t type);
        ~Error(void);

        std::string message(void) const;

    private:

        std::string msg;
        error_t     type;
};

} // namespace webserv

#endif /* ERROR_HPP */
