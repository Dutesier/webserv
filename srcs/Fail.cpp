#include "Fail.hpp"

/* ************************************************************************** */
/* Constructors and Destructors                                               */
/* ************************************************************************** */

Fail::Fail( void ) {}

Fail::Fail(std::string message, std::string filename, unsigned int line)
	: message(message), filename(filename), line(line) {}

Fail::~Fail( void ) {}

/* ************************************************************************** */
/* Getters                                                                    */
/* ************************************************************************** */

std::string	Fail::get_message(void) const { return (this->message); }

std::string	Fail::get_filename(void) const { return (this->filename); }

unsigned int	Fail::get_line(void) const { return (this->line); }

/* ************************************************************************** */
/* Operator's Overload                                                        */
/* ************************************************************************** */

std::ostream&	operator<<(std::ostream& os, Fail const& rhs) {

	if (rhs.get_line())
		os << rhs.get_filename() << "[" << rhs.get_line() << "]: "
		   << rhs.get_message();
	else
		os << rhs.get_filename() << ": " << rhs.get_message();

	return (os);
}
