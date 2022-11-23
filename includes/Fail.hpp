#ifndef FAIL_HPP
#define FAIL_HPP

#include <iostream>

class Fail {

    public:

        /* Constructors and Destructors */
        Fail(void);
        Fail(std::string message, std::string filename, unsigned int line = 0);

        ~Fail(void);

        /* Getters */
        std::string  get_message(void) const;
        std::string  get_filename(void) const;
        unsigned int get_line(void) const;
        std::string  generate_error_str(void) const;

    private:

        /* Private Attributes */
        std::string  message;
        std::string  filename;
        unsigned int line;
};

std::ostream& operator<<(std::ostream& os, Fail const& rhs);

#endif /* FAIL_HPP */
