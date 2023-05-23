#include <gtest/gtest.h>
#include <http/pathValidation.hpp>

class TestPathValidation : public ::testing::Test {
    public:

        TestPathValidation() {}

        ~TestPathValidation() {}

    protected:

        virtual void SetUp() {}

        virtual void TearDown() {}
};

TEST_F(TestPathValidation, test_fullPath) {
    // Test if full path is formatted correctly when directory does not end with
    // / and pathInDir does not start with /
    std::string directory = "webserv/tests/test_web";
    std::string pathInDir = "index.html";
    std::string formattedFullPath =
        webserv::path::formattedFullPath(directory, pathInDir);
    EXPECT_EQ(formattedFullPath, "webserv/tests/test_web/index.html");

    // Test if full path is formatted correctly when directory ends with / and
    // pathInDir starts with /
    directory = "webserv/tests/test_web/";
    pathInDir = "/index.html";
    formattedFullPath = webserv::path::formattedFullPath(directory, pathInDir);
    EXPECT_EQ(formattedFullPath, "webserv/tests/test_web/index.html");

    // Test if full path is formatted correctly when directory ends with / and
    // pathInDir does not start with /
    directory = "webserv/tests/test_web/";
    pathInDir = "index.html";
    formattedFullPath = webserv::path::formattedFullPath(directory, pathInDir);
    EXPECT_EQ(formattedFullPath, "webserv/tests/test_web/index.html");
}