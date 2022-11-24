#include "Config.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(test_Config, constructor) {
    char*            argv[2] = {"webserv", "./default"};
    webserv::Config* config;
    ASSERT_THROW(config = new webserv::Config(2, argv),
                 webserv::Config::InvalidFileException);
    ASSERT_NO_THROW(config = new webserv::Config(1, argv));
    delete config;
}

TEST(test_Config, destructor) {
    char*            argv[2] = {"webserv", "./default"};
    webserv::Config* config;
    ASSERT_THROW(config = new webserv::Config(2, argv),
                 webserv::Config::InvalidFileException);
}

TEST(test_ServerBlock, constructor) {
    char*            argv[1] = {"webserv"};
    webserv::Config* config;
    ASSERT_NO_THROW(config = new webserv::Config(2, argv));
    delete config;
}

TEST(test_ServerBlock, destructor) {
    char*            argv[1] = {"webserv"};
    webserv::Config* config;
    ASSERT_NO_THROW(config = new webserv::Config(2, argv));
    delete config;
}
