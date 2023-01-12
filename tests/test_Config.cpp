#define GTEST_TESTING 1

#include "server/Config.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#define ARGV(X)                                                                \
 char* argv[2];                                                                \
 argv[0] = (char*)"webserv";                                                   \
 argv[1] = (char*)X;                                                           \
 errno = 0

#define IMPL(X)                                                                \
 char* argv[2];                                                                \
 argv[0] = (char*)"webserv";                                                   \
 argv[1] = (char*)X;                                                           \
 errno = 0;                                                                    \
 webserv::Config::impl* dontUse;                                               \
 dontUse = new webserv::Config::impl(2, argv);                                 \
 std::shared_ptr<webserv::Config::impl> m_impl(dontUse)

TEST(test_ServerConfig, constructor) {

    ARGV("../tests/test_file");
    webserv::Config*                 dontUse = new webserv::Config(2, argv);
    std::shared_ptr<webserv::Config> server(dontUse);
}

TEST(test_ServerConfig, destructor) {

    ARGV("../tests/test_file");
    webserv::Config* server = new webserv::Config(2, argv);
    delete server;
}

TEST(test_ServerConfig, InvalidFileException) {

    ARGV("../test_file");
    ASSERT_THROW(webserv::Config a(2, argv),
                 webserv::Config::impl::InvalidFileException);
}

TEST(test_impl, constructor) {

    ARGV("../tests/test_file");
    webserv::Config::impl* dontUse = new webserv::Config::impl(2, argv);
    std::shared_ptr<webserv::Config::impl> m_impl(dontUse);
}

TEST(test_impl, destructor) {

    ARGV("../tests/test_file");
    webserv::Config::impl* server = new webserv::Config::impl(2, argv);
    delete server;
}

TEST(test_ServerConfig, server) {

    IMPL("../tests/test_file");
    ASSERT_EQ(m_impl->server("\tlisten 80").second, "invalid end");
    ASSERT_EQ(m_impl->server("\tserver server_name;").second,
              "server: unknown command");
    ASSERT_TRUE(m_impl->server("\tlisten example.com:8080;").first);
    ASSERT_EQ(m_impl->m_config.back()->m_port, 8080);
    ASSERT_EQ(m_impl->m_config.back()->m_host, "example.com");
}

TEST(test_ServerConfig, location) {

    IMPL("../tests/test_file");

    // creating a LocationBlock
    webserv::LocationBlock* du = new webserv::LocationBlock("php/");
    smt::shared_ptr<webserv::LocationBlock> loc(du);
    m_impl->m_config.back()->m_location.push_back(loc);

    ASSERT_EQ(m_impl->location("\tfastcgi_pass unix").second, "invalid end");
    ASSERT_EQ(m_impl->location("\trot /var/www/html;").second,
              "rot: unknown command");
    ASSERT_TRUE(m_impl->location("\tfastcgi_pass php-fpm;").first);
    ASSERT_EQ(m_impl->m_config.back()->m_location.back()->m_cgi, "php-fpm");
}

TEST(test_ServerConfig, cmd_listen) {

    IMPL("../tests/test_file");

    typedef std::pair<bool, std::string> result_type;

    std::vector<std::string> cmd(1, "listen");

    ASSERT_EQ(m_impl->cmd_listen(cmd).second, "wrong number of arguments");
    cmd.push_back("8080:localhost");
    ASSERT_EQ(m_impl->cmd_listen(cmd).second, "localhost: is not a valid port");
    cmd[1] = "808080";
    ASSERT_EQ(m_impl->cmd_listen(cmd).second, "808080: is not a valid port");
    cmd[1] = "example.com:443";
    ASSERT_TRUE(m_impl->cmd_listen(cmd).first)
        << m_impl->cmd_listen(cmd).second;
    ASSERT_EQ(m_impl->m_config.back()->m_port, 443);
    ASSERT_EQ(m_impl->m_config.back()->m_host, "example.com");
}

TEST(test_ServerConfig, server_name) {

    IMPL("../tests/test_file");

    typedef std::pair<bool, std::string> result_type;

    std::vector<std::string> cmd(1, "server_name");

    // clearing m_srv_name vector
    m_impl->m_config.back()->m_srv_name.clear();

    ASSERT_EQ(m_impl->cmd_server_name(cmd).second, "wrong number of arguments");

    cmd.push_back("example.com");
    cmd.push_back("www.example.com");
    ASSERT_TRUE(m_impl->cmd_server_name(cmd).first)
        << m_impl->cmd_server_name(cmd).second;

    std::string              arr[2]{"example.com", "www.example.com"};
    std::vector<std::string> comp(arr, arr + 2);
    ASSERT_EQ(m_impl->m_config.back()->m_srv_name, comp);
}

TEST(test_ServerConfig, error_page) {

    IMPL("../tests/test_file");

    typedef std::pair<bool, std::string> result_type;

    std::vector<std::string>   cmd(1, "error_page");
    std::map<int, std::string> comp;
    comp[500] = "50X.html";
    comp[501] = "50X.html";
    comp[404] = "404.html";

    // clearing m_srv_name vector
    m_impl->m_config.back()->m_error_page.clear();

    ASSERT_EQ(m_impl->cmd_error_page(cmd).second, "wrong number of arguments");

    cmd.push_back("status");
    ASSERT_EQ(m_impl->cmd_error_page(cmd).second, "wrong number of arguments");

    cmd.push_back("501");
    ASSERT_EQ(m_impl->cmd_error_page(cmd).second,
              "../webserv/website/501: failed to open");

    cmd.push_back("50X.html");
    ASSERT_EQ(m_impl->cmd_error_page(cmd).second,
              "status: invalid status code");

    cmd[1] = "500";
    ASSERT_TRUE(m_impl->cmd_error_page(cmd).first)
        << m_impl->cmd_error_page(cmd).second;

    cmd[1] = "404";
    cmd[2] = "404.html";
    cmd.pop_back();
    ASSERT_TRUE(m_impl->cmd_error_page(cmd).first)
        << m_impl->cmd_error_page(cmd).second;

    ASSERT_EQ(m_impl->m_config.back()->m_error_page, comp);
}

TEST(test_ServerConfig, client_max_body_size) {

    IMPL("../tests/test_file");

    typedef std::pair<bool, std::string> result_type;

    std::vector<std::string> cmd(1, "client_max_body_size");

    ASSERT_EQ(m_impl->cmd_max_size(cmd).second, "wrong number of arguments");

    cmd.push_back("size");
    ASSERT_EQ(m_impl->cmd_max_size(cmd).second, "size: invalid size");

    cmd[1] = "2097153";
    ASSERT_EQ(m_impl->cmd_max_size(cmd).second, "2097153: invalid size");

    cmd[1] = "2097";
    ASSERT_TRUE(m_impl->cmd_max_size(cmd).first)
        << m_impl->cmd_max_size(cmd).second;
}

TEST(test_ServerConfig, cmd_root) {

    IMPL("../tests/test_file");

    typedef std::pair<bool, std::string> result_type;

    std::vector<std::string> cmd(1, "root");

    ASSERT_EQ(m_impl->cmd_root(cmd).second, "wrong number of arguments");

    cmd.push_back("./hellow/");
    ASSERT_EQ(m_impl->cmd_root(cmd).second, "./hellow/: failed to open");

    errno = 0;
    cmd[1] = "../webserv/";
    ASSERT_TRUE(m_impl->cmd_root(cmd).first) << m_impl->cmd_root(cmd).second;
    ASSERT_EQ(m_impl->m_config.back()->m_root, "../webserv/");

    cmd[1] = "../webserv/test_files/";
    ASSERT_TRUE(m_impl->cmd_root(cmd).first) << m_impl->cmd_root(cmd).second;
    ASSERT_EQ(m_impl->m_config.back()->m_root, "../webserv/test_files/");
}

TEST(test_ServerConfig, autoindex) {

    IMPL("../tests/test_file");

    typedef std::pair<bool, std::string> result_type;

    std::vector<std::string> cmd(1, "autoindex");

    ASSERT_EQ(m_impl->cmd_autoindex(cmd).second, "wrong number of arguments");

    cmd.push_back("of");
    ASSERT_EQ(m_impl->cmd_autoindex(cmd).second, "of: unrecognized syntax");

    cmd[1] = "on";
    ASSERT_TRUE(m_impl->cmd_autoindex(cmd).first)
        << m_impl->cmd_autoindex(cmd).second;
    ASSERT_TRUE(m_impl->m_config.back()->m_autoidx);

    cmd[1] = "off";
    ASSERT_TRUE(m_impl->cmd_autoindex(cmd).first)
        << m_impl->cmd_autoindex(cmd).second;
    ASSERT_FALSE(m_impl->m_config.back()->m_autoidx);
}

TEST(test_ServerConfig, index) {

    IMPL("../tests/test_file");

    typedef std::pair<bool, std::string> result_type;

    std::vector<std::string> cmd(1, "index");

    std::string              arr[2] = {"index", "index.html"};
    std::vector<std::string> comp(arr, arr + 2);

    ASSERT_EQ(m_impl->cmd_index(cmd).second, "wrong number of arguments");

    cmd.push_back("index.html");
    ASSERT_TRUE(m_impl->cmd_index(cmd).first) << m_impl->cmd_index(cmd).second;
    ASSERT_EQ(m_impl->m_config.back()->m_idx, comp);

    cmd.push_back("index.php");
    ASSERT_TRUE(m_impl->cmd_index(cmd).first) << m_impl->cmd_index(cmd).second;
    comp.push_back("index.php");
    ASSERT_EQ(m_impl->m_config.back()->m_idx, comp);
}

TEST(test_ServerConfig, lroot) {

    IMPL("../tests/test_file");

    typedef std::pair<bool, std::string> result_type;

    // creating a LocationBlock
    webserv::LocationBlock* du = new webserv::LocationBlock("php/");
    smt::shared_ptr<webserv::LocationBlock> loc(du);
    m_impl->m_config.back()->m_location.push_back(loc);

    std::vector<std::string> cmd(1, "root");

    // inserting a previous root in server_block;
    cmd.push_back("../webserv/");
    ASSERT_TRUE(m_impl->cmd_root(cmd).first) << m_impl->cmd_root(cmd).second;
    cmd.pop_back();

    ASSERT_EQ(m_impl->cmd_lroot(cmd).second, "wrong number of arguments");

    cmd.push_back("test_file/");
    ASSERT_EQ(m_impl->cmd_lroot(cmd).second,
              "../webserv/test_file/: failed to open");

    errno = 0;
    cmd[1] = "test_files/";
    ASSERT_TRUE(m_impl->cmd_lroot(cmd).first) << m_impl->cmd_lroot(cmd).second;
    ASSERT_EQ(m_impl->m_config.back()->m_location.back()->m_root,
              "../webserv/test_files/");
}

TEST(test_ServerConfig, fastcgi_pass) {

    IMPL("../tests/test_file");

    typedef std::pair<bool, std::string> result_type;

    // creating a LocationBlock
    webserv::LocationBlock* du = new webserv::LocationBlock("php/");
    smt::shared_ptr<webserv::LocationBlock> loc(du);
    m_impl->m_config.back()->m_location.push_back(loc);

    std::vector<std::string> cmd(1, "fastcgi_pass");

    ASSERT_EQ(m_impl->cmd_fastcgi_pass(cmd).second,
              "wrong number of arguments");

    ASSERT_EQ(m_impl->cmd_fastcgi_pass(cmd).second,
              "wrong number of arguments");

    cmd.push_back("php-fpm");
    ASSERT_TRUE(m_impl->cmd_fastcgi_pass(cmd).first)
        << m_impl->cmd_fastcgi_pass(cmd).second;
    ASSERT_EQ(m_impl->m_config.back()->m_location.back()->m_cgi, "php-fpm");
}

// TEST(test_ServerConfig, request_method) {
//
// 	typedef std::pair<bool, std::string> result_type;
//
//     std::vector<std::string> cmd(1, "request_method");
//
// 	ASSERT_EQ(m_impl->cmd_request_method(cmd).second, "wrong number of
// arguments");
//
//     // this->impl->server.back()->location.push_back(
//     //     new webserv::LocationConfig("/php"));
//     // webserv::LocationConfig* location =
//     //     this->impl->server.back()->location.back();
//     // std::vector<std::string> request_method(3, "GET");
//     // request_method[1] = "POST";
//     // request_method[2] = "DELETE";
//     // ASSERT_EQ(location->request_method, request_method);
//     // std::vector<std::string> cmd(1, "request_method");
//     // ASSERT_FALSE(this->impl->cmd_request_method(cmd));
//     // cmd.push_back("SET");
//     // ASSERT_FALSE(this->impl->cmd_request_method(cmd));
//     // cmd[1] = "GET";
//     // ASSERT_TRUE(this->impl->cmd_request_method(cmd));
//     // request_method.pop_back();
//     // request_method.pop_back();
//     // ASSERT_EQ(location->request_method, request_method);
// }

TEST(test_ServerConfig, default_values) {

    webserv::ServerBlock server;
    ASSERT_FALSE(server.m_autoidx);
    ASSERT_EQ(server.m_bsize, 1048576);
    ASSERT_EQ(server.m_port, 80);
    ASSERT_EQ(server.m_host, "localhost");
    ASSERT_EQ(server.m_root, "./webserv/website/");
    std::string              arr1[2] = {"index", "index.html"};
    std::vector<std::string> comp(arr1, arr1 + 2);
    ASSERT_EQ(server.m_idx, comp);
    ASSERT_TRUE(server.m_srv_name.empty());
    ASSERT_TRUE(server.m_error_page.empty());
    ASSERT_TRUE(server.m_location.empty());

    std::string            arr2[3] = {"GET", "POST", "DELETE"};
	comp.clear(); comp.insert(comp.begin(), arr2, arr2 + 3);
    webserv::LocationBlock location("uri");
	ASSERT_EQ(location.m_uri, "uri");
	ASSERT_EQ(location.m_root, "");
	ASSERT_EQ(location.m_cgi, "");
	ASSERT_EQ(location.m_req_methods, comp);
}

TEST(test_impl, InvalidFileException) {

    ARGV("../test_file");
    ASSERT_THROW(webserv::Config::impl a(2, argv),
                 webserv::Config::impl::InvalidFileException);
}

TEST(test_impl, InvalidSyntaxException) {

    ARGV("../webserv/test_files/fail_autoindex0");
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_autoindex1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_autoindex2";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_comma0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_comma1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_comma2";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_comma0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_command1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_error_page0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_error_page1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_error_page2";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_index0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_listen0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_listen1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_listen2";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_max_size0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_max_size1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_root0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_server0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_server1";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/fail_server_name0";
    ASSERT_THROW(webserv::Config(2, argv),
                 webserv::Config::impl::InvalidSyntaxException);
    argv[1] = (char*)"../webserv/test_files/no_fail";
    ASSERT_NO_THROW(webserv::Config(2, argv));
}
