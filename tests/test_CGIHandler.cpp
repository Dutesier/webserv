#include "cgi/CGIHandler.hpp"
#include <gtest/gtest.h>

TEST(CGIHandler, FileValidity){
    cgi::CGIHandler cgi(cgi::PYTHON, "tests");
    smt::shared_ptr<HTTPRequest> req = smt::make_shared(new HTTPRequest());
    req->setResource("../tests");
    EXPECT_FALSE(cgi.run(req, 3));
    req->setResource("/../tests");
    EXPECT_FALSE(cgi.run(req, 3));
    req->setResource("/../tests/./pass.php");
    EXPECT_FALSE(cgi.run(req, 3));
    req->setResource("../tests/..//something");
    EXPECT_FALSE(cgi.run(req, 1));
    req->setResource("/test_py/you.py");
    EXPECT_TRUE(cgi.run(req, 1));
    req->setResource("/test_php/../test_py/you.py?key=value");
    EXPECT_TRUE(cgi.run(req, 1));
    req->setResource("test_php/../../srcs");
    EXPECT_FALSE(cgi.run(req, 3));
    req->setResource("/test_php/../test_php/../../srcs");
    EXPECT_FALSE(cgi.run(req, 3));
}