#include "HTTPResponse.hpp"

#include <gtest/gtest.h>
#include <iostream>

class test_Response : public ::testing::Test {
    public:

        void SetUp(void) {
            char* argv[1] = {"webserv"};
            this->impl = new webserv::Config::impl(1, argv);
        }

        void TearDown(void) { delete this->impl; }

    protected:

        webserv::Config::impl* impl;
};
