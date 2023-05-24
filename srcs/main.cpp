#include "webserv.hpp"

#include <iostream>

int main(int argc, char* argv[]) {

    webserv::webserv(argc, argv);
    return (0);
}

// TODO:
// - Set max_body_size depending on the server block
// (exposed in the HTTPParser class)