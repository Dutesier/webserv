#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "Config.hpp"

namespace webserv {

class HTTPServer {

    public:

        /* Constructors and Destructors */
        HTTPServer(int argc, char* argv[]);
        ~HTTPServer(void);

        /* Other Functions */
        void start(void);
        void run(void);
        void stop(void);

    private:

        /* Private Attributes */
        Config* config;
};

} // namespace webserv

#endif /* HTTP_SERVER_HPP */
