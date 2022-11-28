#define GTEST_TESTING 1

#include "Config.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <vector>

// TEST(test_Config, constructor) {
//     char*            argv[2] = {"webserv", "./default"};
//     webserv::Config* config;
//     ASSERT_THROW(config = new webserv::Config(2, argv),
//                  webserv::Config::InvalidFileException);
//     ASSERT_NO_THROW(config = new webserv::Config(1, argv));
//     delete config;
// }
//
// TEST(test_Config, destructor) {
//     char*            argv[2] = {"webserv", "./default"};
//     webserv::Config* config;
//     ASSERT_THROW(config = new webserv::Config(2, argv),
//                  webserv::Config::InvalidFileException);
// }

class test_Config : public ::testing::Test {
    public:

        void SetUp(void) {
            char* argv[1] = {"webserv"};
            this->impl = new webserv::Config::impl(1, argv);
        }

        void TearDown(void) { delete this->impl; }

    protected:

        webserv::Config::impl* impl;
};

TEST_F(test_Config, constructor) {}

TEST_F(test_Config, destructor) {}
