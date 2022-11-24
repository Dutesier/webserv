#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

namespace webserv {

class Error {

    public:

        /* Member Types */
        typedef enum error { invalid_file, invalid_syntax } error_t;

        /* Constructors and Destructors */
        Error(std::string msg, error_t type);
        ~Error(void);

        /* Getters */
        std::string message(void) const;

    private:

        /* Private Attributes */
        std::string msg;
        error_t     type;
};

} // namespace webserv

#endif /* ERROR_HPP */
