#ifndef CONFIG_DIRECTIVES_HPP
#define CONFIG_DIRECTIVES_HPP

#include "cgi/CgiHandler.hpp"
#include "config/Line.hpp"
#include "config/ServerOpts.hpp"
#include "http/methods.hpp"
#include "utils/ft_filesystem.hpp"
#include "utils/ft_string.hpp"
#include "utils/Host.hpp"
#include "utils/Logger.hpp"
#include "utils/smt.hpp"

#include <map>
#include <set>
#include <string>
#include <typeinfo>
#include <vector>

namespace config {

class DirectiveTypeTraitsBase {
    public:

        DirectiveTypeTraitsBase(std::string const& name);
        virtual ~DirectiveTypeTraitsBase(void);

        bool operator==(DirectiveTypeTraitsBase const& rhs) const;

        virtual void set(std::vector<ServerOpts>&) const = 0;
        virtual void set(ServerOpts&) const = 0;
        virtual void set(LocationOpts&) const = 0;

        std::string const getName(void);

        virtual bool isValid(void) const = 0;
        virtual bool isBlockDirective(void) const = 0;
        virtual bool isRouteDirective(void) const = 0;

        std::string const m_name;
};

template<LineType Directive>
struct DirectiveTypeTraits;

template<>
struct DirectiveTypeTraits<LineBlock> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_SERVER), m_valid(false) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != "server") {
                LOG_W(getName() << ": invalid directive.");
                return;
            }
            if (args[1] != "{") {
                LOG_W(getName() << ": invalid end.");
                return;
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>& opts) const {
            opts.push_back(ServerOpts());
        }

        void set(ServerOpts&) const {}

        void set(LocationOpts&) const {}

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (false); }

        bool isRouteDirective(void) const { return (false); }

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<LineRoute> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_LOCATION), m_valid(false) {
            if (args.size() != 3) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != "location") {
                LOG_W(getName() << ": invalid directive.");
                return;
            }
            if (args[2] != "{") {
                LOG_W(getName() << ": invalid end.");
                return;
            }
            m_target = (args[1][0] != '/') ? ("/" + args[1]) : args[1];
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {}

        void set(ServerOpts& opts) const {
            opts.m_locations.push_back(LocationOpts(m_target));
        }

        void set(LocationOpts&) const {}

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (false); }

        bool        m_valid;
        std::string m_target;
};

template<>
struct DirectiveTypeTraits<LineEnd> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_END), m_valid(false) {
            if (args.size() != 1) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != "}") {
                LOG_W(getName() << ": invalid directive.");
                return;
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {}

        void set(ServerOpts&) const {}

        void set(LocationOpts&) const {}

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<LineListen> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_LISTEN), m_valid(false) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            size_t pos = args[1].find(":");
            if (pos != std::string::npos) {
                if (!pos || pos == args[1].size() - 1) {
                    LOG_W(getName() << ": invalid host:port.");
                    return;
                }
                m_port = args[1].substr(0, pos);
                m_host = args[1].substr(pos + 1);
                if (!ft::string::isnumeric(m_port)) {
                    LOG_W(getName() << ": invalid host:port.");
                    return;
                }
            }
            else if (ft::string::isnumeric(args[1])) {
                m_host = "localhost";
                m_port = args[1];
            }
            else {
                m_port = "8080";
                m_host = args[1];
            }

            // resolv host name
            std::vector<std::string> resolvHost =
                net::Host(m_host).resolve(m_port);
            if (resolvHost.empty()) {
                LOG_W(getName() << ": failed to resolv host.");
                return;
            }
            m_host = resolvHost[0];

            m_valid = true;
        }

        void set(std::vector<ServerOpts>&) const {}

        void set(ServerOpts& opts) const {
            opts.m_host = m_host;
            opts.m_port = m_port;
        }

        void set(LocationOpts&) const {}

        ~DirectiveTypeTraits(void) {}

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (false); }

        bool m_valid;

        std::string m_host;
        std::string m_port;
};

template<>
struct DirectiveTypeTraits<LineServerName> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_SERVER_NAME), m_valid(false) {

            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            m_server_name = args[1];
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {}

        void set(ServerOpts& opts) const { opts.m_server_name = m_server_name; }

        void set(LocationOpts&) const {}

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (false); }

        bool m_valid;

        std::string m_server_name;
};

template<>
struct DirectiveTypeTraits<LineRoot> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_ROOT), m_valid(false) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            m_root = args[1];
            if (!m_root.isValid()) {
                LOG_W(getName() << ": invalid root.");
                return;
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {}

        void set(ServerOpts& opts) const { opts.m_root = m_root; }

        void set(LocationOpts& opts) const { opts.m_root = m_root; }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        ft::directory m_root;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<LineErrorPage> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_ERROR_PAGE), m_valid(false) {
            if (args.size() < 3) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            ft::file file = args.back();
            if (!file.isValid()) {
                LOG_W(getName() << ": invalid file.");
                return;
            }

            std::vector<std::string> error_codes =
                std::vector<std::string>(args.begin() + 1, args.end() - 1);

            for (std::vector<std::string>::iterator it = error_codes.begin();
                 it != error_codes.end(); ++it) {
                if (!ft::string::isnumeric(*it)) {
                    LOG_W(getName() << ": invalid error code.");
                    return;
                }
                m_error_pages[ft::string::stoi(*it)] = file;
            }

            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {}

        void set(ServerOpts& opts) const { opts.m_error_pages = m_error_pages; }

        void set(LocationOpts& opts) const {
            opts.m_error_pages = m_error_pages;
        }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        std::map<int, ft::file> m_error_pages;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<LineMaxBodySize> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_MAX_BODY_SIZE), m_max_body_size(0),
              m_valid(false) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            if (!ft::string::isnumeric(args[1])) {
                LOG_W(getName() << ": invalid max body size.");
                return;
            }
            m_max_body_size = ft::string::stoul(args[1]);
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {}

        void set(ServerOpts& opts) const {
            opts.m_max_body_size = m_max_body_size;
        }

        void set(LocationOpts& opts) const {
            opts.m_max_body_size = m_max_body_size;
        }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        unsigned long m_max_body_size;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<LineAllowMethods> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_ALLOW_METHODS), m_valid(false) {
            if (args.size() < 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            for (std::vector<std::string>::const_iterator it = args.begin() + 1;
                 it != args.end(); ++it) {
                if (http::convertMethod(*it) == http::UNKNOWN_METHOD) {
                    LOG_W(getName() << ": invalid method.");
                    return;
                }
                m_allowed_methods.insert(*it);
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {}

        void set(ServerOpts& opts) const {
            opts.m_allowed_methods = m_allowed_methods;
        }

        void set(LocationOpts& opts) const {
            opts.m_allowed_methods = m_allowed_methods;
        }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        std::set<std::string> m_allowed_methods;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<LineIndex> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_INDEX), m_valid(false) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            m_index = args[1];
            if (!m_index.isValid()) {
                LOG_W(getName() << ": invalid index file.");
                return;
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {}

        void set(ServerOpts&) const {}

        void set(LocationOpts& ops) const { ops.m_index = m_index; }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (false); }

        bool isRouteDirective(void) const { return (true); }

        ft::file m_index;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<LineAutoIndex> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_AUTOINDEX), m_autoindex(false),
              m_valid(false) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            if (args[1] == "on") { m_autoindex = true; }
            else if (args[1] == "off") { m_autoindex = false; }
            else {
                LOG_W(getName() << ": invalid autoindex value.");
                return;
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {}

        void set(ServerOpts&) const {}

        void set(LocationOpts& ops) const { ops.m_autoindex = m_autoindex; }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (false); }

        bool isRouteDirective(void) const { return (true); }

        bool m_autoindex;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<LineCgiExtension> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_FASTCGI), m_valid(false) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            if (cgi::convertCgiExtension(args[1]) == cgi::Unknown) {
                LOG_W(getName() << ": invalid cgi extension.");
                return;
            }
            m_cgi_extension = args[1];
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {}

        void set(ServerOpts&) const {}

        void set(LocationOpts& ops) const {
            ops.m_cgi_extension = m_cgi_extension;
        }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (false); }

        bool isRouteDirective(void) const { return (true); }

        std::string m_cgi_extension;

        bool m_valid;
};

} // namespace config

#endif /* CONFIG_DIRECTIVES_HPP */