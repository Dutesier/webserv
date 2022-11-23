#include "Fail.hpp"

Fail::Fail(void) {}

Fail::Fail(std::string message, std::string filename, unsigned int line)
    : message(message), filename(filename), line(line) {}

Fail::~Fail(void) {}

std::string Fail::generate_error_str(void) const {
    return get_filename() + "[" + std::to_string(get_line()) +
           "]: " + get_message();
}

std::string Fail::get_message(void) const { return (this->message); }

std::string Fail::get_filename(void) const { return (this->filename); }

unsigned int Fail::get_line(void) const { return (this->line); }

std::ostream& operator<<(std::ostream& os, Fail const& rhs) {

    if (rhs.get_line())
        os << rhs.get_filename() << "[" << rhs.get_line()
           << "]: " << rhs.get_message();
    else os << rhs.get_filename() << ": " << rhs.get_message();

    return (os);
}
