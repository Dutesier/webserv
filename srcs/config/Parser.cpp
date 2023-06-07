#include "config/Parser.hpp"

#include "config/DirectiveTypeTraits.hpp"
#include "config/Line.hpp"
#include "config/Options.hpp"
#include "utils/ft_exceptions.hpp"
#include "utils/Logger.hpp"

#include <iostream>

namespace config {

Parser::Parser(ft::file filename) : m_line(), m_pos(0) {

    if (!filename.isValid()) { throw ft::InvalidFileException(); }
    m_file.open(std::string(filename).c_str());
    if (!m_file.is_open()) { throw ft::FailedToOpenFileException(); }
}

Parser::~Parser(void) { m_file.close(); }

bool Parser::nextLine(void) {
    if (m_file.eof()) { return (false); }
    std::getline(m_file, m_line);
    m_pos++;
    return (true);
}

std::string Parser::getLine(void) const { return (m_line); }

size_t Parser::getPosition(void) const { return (m_pos); }

std::vector<ServerOpts> const& Parser::getOptions(void) const {
    return (m_directives);
}

void Parser::error(void) const {
    LOG_E("config: syntax error: " << m_line << " (:" << m_pos << ")");
    throw InvalidSyntaxException();
}

template<LineType T>
void Parser::parseLine(std::vector<std::string> const& args, int level) {

    DirectiveTypeTraits<T> directive(args);
    if (!directive.isValid()) { error(); }

    if (level == 0) {
        if (directive.isBlockDirective() || directive.isRouteDirective()) {
            LOG_W("config: directive " << directive.getName()
                                       << " is not a global directive.");
            error();
        }
        directive.set(m_directives);
    }
    if (level == 1) {
        if (!directive.isBlockDirective()) {
            LOG_W("config: directive " << directive.getName()
                                       << " is not a server directive.");
            error();
        }
        directive.set(m_directives.back());
    }
    else if (level == 2) {
        if (!directive.isRouteDirective()) {
            LOG_W("config: directive " << directive.getName()
                                       << " is not a location directive.");
            error();
        }
        directive.set(m_directives.back().m_locations.back());
    }
}

char const* Parser::InvalidSyntaxException::what(void) const throw() {
    return ("config::parser: invalid syntax.");
}

void parse(ft::file const& filename) {
    Parser parser(filename);
    int    level = 0;
    while (parser.nextLine()) {
        Line line(parser.getLine());
        switch (line.getType()) {
            case (LineEmpty): break;

            case (LineBlock):
                parser.parseLine<LineBlock>(line.format(), level);
                level++;
                break;

            case (LineRoute):
                parser.parseLine<LineRoute>(line.format(), level);
                level++;
                break;

            case (LineEnd):
                parser.parseLine<LineEnd>(line.format(), level);
                level--;
                break;

            case (LineListen):
                parser.parseLine<LineListen>(line.format(), level);
                break;

            case (LineServerName):
                parser.parseLine<LineServerName>(line.format(), level);
                break;

            case (LineRoot):
                parser.parseLine<LineRoot>(line.format(), level);
                break;

            case (LineErrorPage):
                parser.parseLine<LineErrorPage>(line.format(), level);
                break;

            case (LineMaxBodySize):
                parser.parseLine<LineMaxBodySize>(line.format(), level);
                break;

            case (LineAllowMethods):
                parser.parseLine<LineAllowMethods>(line.format(), level);
                break;

            case (LineIndex):
                parser.parseLine<LineIndex>(line.format(), level);
                break;

            case (LineAutoIndex):
                parser.parseLine<LineAutoIndex>(line.format(), level);
                break;

            case (LineCgiExtension):
                parser.parseLine<LineCgiExtension>(line.format(), level);
                break;

            case (LineUnknown):
            default: parser.error();
        }
    }
    if (level != 0) { parser.error(); }
    // initializing options
    Options::getInstance(parser.getOptions());
}

} // namespace config
