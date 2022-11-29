#include "HTTPServer.hpp"

#include <gtest/gtest.h>
#include <iostream>

// class test_HTTPServer : public testing::Test {
//     public:
//
//         void SetUp(void) {
//             char* argv[2] = {"webserv", "default"};
//             this->server = new webserv::HTTPServer(2, argv);
//         }
//
//         void TearDown(void) { delete this->server; }
//
//     protected:
//
//         webserv::HTTPServer* server;
// };
//
// TEST_F(test_HTTPServer, constructor) {}
//
// TEST_F(test_HTTPServer, destructor) {}
//
// TEST_F(test_HTTPServer, start) { ASSERT_NO_THROW(this->server->start()); }
//
// TEST_F(test_HTTPServer, run) { ASSERT_NO_THROW(this->server->start()); }
//
// TEST_F(test_HTTPServer, stop) { ASSERT_NO_THROW(this->server->run()); }
