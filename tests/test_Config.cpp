#define GTEST_TESTING 1

#include "config/Config.hpp"

#include <gtest/gtest.h>

#define ARGV(X)                                                                \
 char* argv[2];                                                                \
 argv[0] = (char*)"webserv";                                                   \
 argv[1] = (char*)X;                                                           \
 errno = 0

TEST(test_Config, constructor) {

    ARGV("../tests/test_web/default");
    webserv::Config* server;
    ASSERT_NO_THROW(server = new webserv::Config(2, argv));
    delete server;
}

TEST(test_Config, destructor) {

    ARGV("../tests/test_web/default");
    webserv::Config* server = new webserv::Config(2, argv);
    ASSERT_NO_THROW(delete server);
}

TEST(test_Config, InvalidFileException) {

    ARGV("../test_web/default");
    ASSERT_THROW(webserv::Config a(2, argv),
                 webserv::Config::InvalidFileException);
}

TEST(test_impl, InvalidSyntaxException) {

    ARGV("../tests/test_web/test_files/fail_autoindex0");
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_autoindex1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_autoindex2";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_comma0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_comma1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_comma2";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_comma0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_command1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_error_page0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_error_page1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_error_page2";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_listen0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_listen1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_listen2";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_max_size0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_max_size1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_root0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_server0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_server1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/fail_server_name0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::InvalidSyntaxException);
    argv[1] = (char*)"../tests/test_web/test_files/no_fail";
    ASSERT_NO_THROW(webserv::Config(2, argv));
}

TEST(test_Config, values) {

    ARGV("../tests/test_web/test_files/no_fail");
    smt::shared_ptr<webserv::Config> config(new webserv::Config(2, argv));

    // testing first server block
    std::map<int, std::string> comp1;
    comp1[500] = "../tests/test_web/500.html";
    comp1[501] = "../tests/test_web/500.html";
    comp1[502] = "../tests/test_web/500.html";
    comp1[503] = "../tests/test_web/500.html";

    smt::shared_ptr<webserv::ServerBlock> server = config->m_blocks[0];
    ASSERT_TRUE(server->m_autoindex);
    ASSERT_EQ(server->m_body_size, BODY_SIZE);
    ASSERT_EQ(server->m_port, 443);
    ASSERT_EQ(server->m_host, "128.0.0.1");
    ASSERT_EQ(server->m_index, std::string(D_ROOT_PATH) + "index.html");
    ASSERT_EQ(server->m_root, "../tests/test_web/");
    ASSERT_EQ(server->m_server_name, "www.example.com");
    ASSERT_EQ(server->m_error_page, comp1);
    ASSERT_EQ(server->m_location.size(), 2);

    smt::shared_ptr<webserv::LocationBlock> location = server->m_location["two"];
    std::set<std::string>            comp2;
    comp2.insert("GET");
    comp2.insert("POST");
    ASSERT_FALSE(location->m_cgi);
    ASSERT_EQ(location->m_target, "two");
    ASSERT_EQ(location->m_root, "");
    ASSERT_EQ(location->m_allowed_methods, comp2);

    comp2.erase(comp2.find("POST"));
    location = server->m_location["one/two/three"];
    ASSERT_TRUE(location->m_cgi);
    ASSERT_EQ(location->m_target, "one/two/three");
    ASSERT_EQ(location->m_root, "../tests/test_web/test_files");
    ASSERT_EQ(location->m_allowed_methods, comp2);

    // testing second server block
    std::map<int, std::string> comp3;
    comp3[400] = "../tests/test_web/404.html";
    comp3[401] = "../tests/test_web/404.html";
    comp3[402] = "../tests/test_web/404.html";
    server = config->m_blocks[1];
    ASSERT_FALSE(server->m_autoindex);
    ASSERT_EQ(server->m_body_size, 8000);
    ASSERT_EQ(server->m_port, 8081);
    ASSERT_EQ(server->m_host, "*");
    ASSERT_EQ(server->m_index, "../tests/test_web/index.html");
    ASSERT_EQ(server->m_root, "../tests/test_web/");
    ASSERT_EQ(server->m_server_name, "");
    ASSERT_EQ(server->m_error_page, comp3);
    ASSERT_TRUE(server->m_location.empty());

    // testing third server block
    server = config->m_blocks[2];
    comp3.clear();
    ASSERT_TRUE(server->m_autoindex);
    ASSERT_EQ(server->m_body_size, BODY_SIZE);
    ASSERT_EQ(server->m_port, 80);
    ASSERT_EQ(server->m_host, "*");
    ASSERT_EQ(server->m_index, std::string(D_ROOT_PATH) + "index.html");
    ASSERT_EQ(server->m_root, D_ROOT_PATH);
    ASSERT_EQ(server->m_server_name, "");
    ASSERT_EQ(server->m_error_page, comp3);
    ASSERT_EQ(server->m_location.size(), 1);

    comp2.insert("GET");
    comp2.insert("POST");
    comp2.insert("DELETE");
    location = server->m_location["target"];
    ASSERT_FALSE(location->m_cgi);
    ASSERT_EQ(location->m_target, "target");
    ASSERT_EQ(location->m_root, "");
    ASSERT_EQ(location->m_allowed_methods, comp2);

    // testing forth server block
    server = config->m_blocks[3];
    ASSERT_TRUE(server->m_autoindex);
    ASSERT_EQ(server->m_body_size, BODY_SIZE);
    ASSERT_EQ(server->m_port, 80);
    ASSERT_EQ(server->m_host, "*");
    ASSERT_EQ(server->m_index, std::string(D_ROOT_PATH) + "index.html");
    ASSERT_EQ(server->m_root, D_ROOT_PATH);
    ASSERT_EQ(server->m_server_name, "");
    ASSERT_EQ(server->m_error_page, comp3);
    ASSERT_TRUE(server->m_location.empty());
}
