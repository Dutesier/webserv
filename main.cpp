// #include "webserv.hpp"

// #include <iostream>

// int main(int argc, char* argv[]) {
//     webserv::webserv(argc, argv);
//     return (0);
// }

#include "HTTPParser.hpp"
#include <iostream>

int main(){
    HTTPParser parser;
    std::vector<std::string> req;

    std::string s("GET /home/var HTTP/1.1\t\n");
    req.push_back(s);
    smt::shared_ptr<HTTPRequest> result = parser.parse(req);
    if (result) {
        std::cout << result->method << std::endl; 
        std::cout << result->resource << std::endl; 
        std::cout << result->version << std::endl; 
    }
}
