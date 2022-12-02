#include "Error.hpp"

namespace webserv {

Error::Error(std::string msg, error_t type) : msg(msg), type(type) {}

Error::~Error(void) {}

std::string Error::message(void) const {
    switch (this->type) {
        case invalid_file: return ("invalid file: " + this->msg);
        case invalid_syntax: return ("invalid syntax: " + this->msg);
    }
	return ("error: " + this->msg);
}

} // namespace webserv
