#include <gtest/gtest.h>
#include <http/HTTPRequest.hpp>
#include <http/HTTPResponse.hpp>
#include <http/methods.hpp>
#include <unistd.h>

class TestMethods : public ::testing::Test {
    public:

        TestMethods() {}

        ~TestMethods() {}

    protected:

        virtual void SetUp() {
            m_request = smt::make_shared<HTTPRequest>(new HTTPRequest());
            m_location = smt::make_shared<webserv::LocationBlock>(
                new webserv::LocationBlock("/www/block"));
            m_location->m_allowed_methods.insert("GET");
            m_location->m_root = "../tests";
        }

        virtual void TearDown() {}

        smt::shared_ptr<HTTPRequest>            m_request;
        smt::shared_ptr<webserv::LocationBlock> m_location;
};

TEST_F(TestMethods, test_GET) {

    system("touch ../tests/test_web/tmp.txt");
    system("echo \"Hello World\" > ../tests/test_web/tmp.txt");
    // clang-format off
    // clang-format on
    m_request->setResource("/test_web/tmp.txt");
    smt::shared_ptr<webserv::HTTPResponse> response =
        webserv::methods::GET(m_request, m_location);
    EXPECT_EQ(response->m_status, 200);
    EXPECT_EQ(response->m_body, "Hello World\n");
    system("rm ../tests/test_web/tmp.txt");
}

TEST_F(TestMethods, test_GET_directory) {
    // Test GET behavior on a directory with index set and autoindex off where
    // index.html exists
    m_request->setResource("/test_web");
    m_location->m_autoindex = false;

    system("touch ../tests/test_web/tmp.txt");
    system("echo \"Hello World\" > ../tests/test_web/tmp.txt");

    m_location->m_index = "test_web/tmp.txt";

    // clang-format off
    std::string indexHTML = "Hello World\n";
    // clang-format on

    smt::shared_ptr<webserv::HTTPResponse> response =
        webserv::methods::GET(m_request, m_location);
    EXPECT_EQ(response->m_status, 200);
    EXPECT_EQ(response->m_body, indexHTML);

    // Test GET behavior on a directory with index set and autoindex off where
    // index.php does not exist
    m_location->m_autoindex = false;
    m_location->m_index = "index.php";
    response = webserv::methods::GET(m_request, m_location);
    EXPECT_EQ(response->m_status, 403);

    // Test GET behavior on a directory with index set and autoindex on where
    // index.php does not exist
    m_location->m_autoindex = true;
    response = webserv::methods::GET(m_request, m_location);
    EXPECT_EQ(response->m_status, 404);

    // Test GET behavior on a directory with index not set and autoindex on
    m_location->m_autoindex = true;
    m_location->m_index = "";
    response = webserv::methods::GET(m_request, m_location);
    EXPECT_EQ(response->m_status, 200);
    // Expect body to not be empty
    EXPECT_FALSE(response->m_body == "");

    // Test GET behavior on a directory with index not set and autoindex off
    m_location->m_autoindex = false;
    m_location->m_index = "";
    response = webserv::methods::GET(m_request, m_location);
    EXPECT_EQ(response->m_status, 403);
    system("rm ../tests/test_web/tmp.txt");
}

TEST_F(TestMethods, test_POST) {
    // Tests POST behavior
    m_request->setResource("/test_web/deleteFilesInDir/newFile.txt");
    m_request->setContent("Hello World!");
    smt::shared_ptr<webserv::HTTPResponse> response =
        webserv::methods::POST(m_request, m_location);
    EXPECT_EQ(response->m_status, 200);

    // Make a call with GET to check if POST actually created the file
    response = webserv::methods::GET(m_request, m_location);
    EXPECT_EQ(response->m_status, 200);
    EXPECT_EQ(response->m_body, "Hello World!");

    // Make a call with POST to check if POST appends to the file
    m_request->setContent("Hello World!");
    response = webserv::methods::POST(m_request, m_location);
    EXPECT_EQ(response->m_status, 200);

    // Make a call with GET to check if POST actually appended to the file
    response = webserv::methods::GET(m_request, m_location);
    EXPECT_EQ(response->m_status, 200);
    EXPECT_EQ(response->m_body, "Hello World!Hello World!");

    // Delete the file with DELETE
    m_request->setResource("/test_web/deleteFilesInDir/newFile.txt");
    response = webserv::methods::DELETE(m_request, m_location);
    EXPECT_EQ(response->m_status, 200);
}

// Test the DELETE method
TEST_F(TestMethods, test_DELETE) {
    // Use the POST method to create a file to delete
    m_request->setResource("/test_web/deleteFilesInDir/newFile.txt");
    m_request->setContent("Goodbye Cruel World!");
    smt::shared_ptr<webserv::HTTPResponse> response =
        webserv::methods::POST(m_request, m_location);
    EXPECT_EQ(response->m_status, 200);

    // Make a call with DELETE to delete the file
    m_request->setResource("/test_web/deleteFilesInDir/newFile.txt");
    response = webserv::methods::DELETE(m_request, m_location);
    EXPECT_EQ(response->m_status, 200);

    // Make a call with GET to check if DELETE actually deleted the file
    response = webserv::methods::GET(m_request, m_location);
    EXPECT_EQ(response->m_status, 404); // File should not exist

    // Try to delete a file that does not exist
    response = webserv::methods::DELETE(m_request, m_location);
    EXPECT_EQ(response->m_status, 404); // File should not exist

    // Try to delete a directory
    m_request->setResource("/test_web/deleteFilesInDir");
    response = webserv::methods::DELETE(m_request, m_location);
    EXPECT_EQ(response->m_status, 403); // Should not be allowed to delete a
                                        // directory
}