#include "cgi/Argv.hpp"
#include "cgi/CgiHandler.hpp"
#include "cgi/Envp.hpp"
#include "http/Request.hpp"
#include "utils/ft_array.hpp"
#include "utils/Logger.hpp"

#include <gtest/gtest.h>

// TODO: Refactor this with Argv and Envp and better examples and asserts
TEST(testCgiTypes, testConvertType) {
    EXPECT_EQ(cgi::convertCgiExtension(".py"), cgi::Py);
    EXPECT_EQ(cgi::convertCgiExtension(".php"), cgi::Php);
    EXPECT_EQ(cgi::convertCgiExtension(".cgi"), cgi::Cgi);
    EXPECT_EQ(cgi::convertCgiExtension(".other"), cgi::Unknown);
    EXPECT_EQ(cgi::convertCgiExtension("other"), cgi::Unknown);
}

TEST(testSplitInfoFromPath, testAllCases) {

    EXPECT_EQ(
        cgi::splitInfoFromPath("/path/to/script.cgi/param1/param2"),
        std::vector<std::string>({"/path/to/script.cgi", "/param1/param2"}));

    EXPECT_EQ(
        cgi::splitInfoFromPath("/path/to/script/param1/param2.py"),
        std::vector<std::string>({"/path/to/script/param1/param2.py", ""}));

    EXPECT_EQ(cgi::splitInfoFromPath("/path/to/script.conf/param1/param2.py"),
              std::vector<std::string>(
                  {"/path/to/script.conf/param1/param2.py", ""}));

    EXPECT_EQ(
        cgi::splitInfoFromPath("/path/to/script.conf/param2.py/file/info"),
        std::vector<std::string>(
            {"/path/to/script.conf/param2.py", "/file/info"}));
}

char** vectorToCharPointerArray(std::vector<std::string> const& vec) {
    // Allocate memory for char** array
    char** charArray = new char*[vec.size() + 1];

    // Copy each string to char* array
    for (size_t i = 0; i < vec.size(); ++i) {
        // Allocate memory for each char* element
        charArray[i] = new char[vec[i].size() + 1];

        // Copy string data to char* element
        strcpy(charArray[i], vec[i].c_str());
    }

    // Add NULL as the last element
    charArray[vec.size()] = NULL;

    return charArray;
}

TEST(testCgi, testingRequestToIndex) {
    // argv
    char* path = new char[17];
    strcpy(path, "/usr/bin/python3");
    char** argv = vectorToCharPointerArray(
        {"/usr/bin/python3", "./websites/cgi/python/index.py"});

    char** envp = vectorToCharPointerArray(
        {"REQUEST_METHOD=GET", "SCRIPT_NAME=./websites/cgi/python/index.py",
         "HTTP_USER_AGENT=Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
         "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.1234.56 "
         "Safari/537.36"});

    cgi::CgiHandler cgi(path, argv, envp);
    auto            resp = cgi.run();
    LOG_D(resp);
}

TEST(testCgi, testingCreatingAFile) {
    char* path = new char[17];
    strcpy(path, "/usr/bin/python3");

    char** argv = vectorToCharPointerArray(
        {"/usr/bin/python3", "./websites/cgi/python/createFile.py"});

    char** envp = vectorToCharPointerArray(
        {"REQUEST_METHOD=POST", "CONTENT_LENGTH=12", "CONTENT_TYPE=text/plain",
         "SCRIPT_NAME=./websites/cgi/python/createFile.py",
         "HTTP_USER_AGENT=Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
         "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.1234.56 "
         "Safari/537.36"});
    cgi::CgiHandler cgi(path, argv, envp, "Hello World!");
    auto            resp = cgi.run();
    LOG_D(resp);
}

TEST(testCgi, testingCreateFile) {
    char* path = new char[17];
    strcpy(path, "/usr/bin/python3");

    char** argv = vectorToCharPointerArray(
        {"/usr/bin/python3", "./websites/cgi/python/test.py"});

    char** envp = vectorToCharPointerArray(
        {"REQUEST_METHOD=POST", "CONTENT_LENGTH=12", "CONTENT_TYPE=text/plain",
         "SCRIPT_NAME=./websites/cgi/python/test.py",
         "HTTP_USER_AGENT=Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
         "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.1234.56 "
         "Safari/537.36",
         "QUERY_STRING=?param1=value1&param2=value2"});
    cgi::CgiHandler cgi(path, argv, envp);
    auto            resp = cgi.run();
    LOG_D(resp);
}

TEST(testCgi, testCgiWithArgvAndEnvp) {
    char* path = new char[17];
    strcpy(path, "/usr/bin/python3");

    cgi::Argv argv({"/usr/bin/python3", "./websites/cgi/python/test.py"});
    smt::shared_ptr<http::Request> req(
        new http::Request("POST "
                          "/cgi/myscript.py HTTP/1.1\r\n"
                          "Host: example.com\r\n"
                          "Accept-Encoding: gzip, deflate, br\r\n"
                          "Accept-Language: en-US,en;q=0.9\r\n"
                          "Connection: keep-alive\r\n\r\n"));
    cgi::Envp       envp(req);
    cgi::CgiHandler cgi(path, argv, envp);
    auto            resp = cgi.run();
}