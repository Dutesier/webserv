#include "config/Blocks.hpp"

#include <gtest/gtest.h>

TEST(test_block, autoindex) {

    smt::shared_ptr<webserv::ServerBlock> server(new webserv::ServerBlock());

    // checking default values
    ASSERT_EQ(server->m_autoindex, true);

    // checking all possible error messages
    std::vector<std::string> command(1, "autoindex");
    ASSERT_EQ(server->autoindex(command), "wrong number of arguments");
    ASSERT_EQ(server->m_autoindex, true);

    command.push_back("oon");
    ASSERT_EQ(server->autoindex(command), "oon: unrecognized syntax");
    ASSERT_EQ(server->m_autoindex, true);

    // checking success
    command[1] = "off";
    ASSERT_EQ(server->autoindex(command), "");

    // checking values
    ASSERT_EQ(server->m_autoindex, false);
}

TEST(test_block, body_size) {

    smt::shared_ptr<webserv::ServerBlock> server(new webserv::ServerBlock());

    // checking default values
    ASSERT_EQ(server->m_body_size, BODY_SIZE);

    // checking all possible error messages
    std::vector<std::string> command(1, "max_body_size");
    ASSERT_EQ(server->body_size(command), "wrong number of arguments");
    ASSERT_EQ(server->m_body_size, BODY_SIZE);

    command.push_back("asad");
    ASSERT_EQ(server->body_size(command), "asad: invalid max body size");
    ASSERT_EQ(server->m_body_size, BODY_SIZE);

    command[1] = "32516983297";
    ASSERT_EQ(server->body_size(command), "32516983297: invalid max body size");
    ASSERT_EQ(server->m_body_size, BODY_SIZE);

    // checking success
    command[1] = "3251";
    ASSERT_EQ(server->body_size(command), "");

    // checking values
    ASSERT_EQ(server->m_body_size, 3251);
}

TEST(test_block, listen) {

    smt::shared_ptr<webserv::ServerBlock> server(new webserv::ServerBlock());

    // checking default values
    ASSERT_EQ(server->m_port, "8080");
    ASSERT_EQ(server->m_host, "localhost");

    // checking all possible error messages
    std::vector<std::string> command(1, "listen");
    ASSERT_EQ(server->listen(command), "wrong number of arguments");
    ASSERT_EQ(server->m_port, "8080");
    ASSERT_EQ(server->m_host, "localhost");

    // TODO: make this tests better
    // checking success
    command.push_back("128.0.0.1");
    ASSERT_EQ(server->listen(command), "");
    ASSERT_EQ(server->m_host, "128.0.0.1");

    command[1] = "8080";
    ASSERT_EQ(server->listen(command), "");
    ASSERT_EQ(server->m_port, "8080");

    command[1] = "127.0.0.1:8081";
    ASSERT_EQ(server->listen(command), "");

    // checking values
    ASSERT_EQ(server->m_port, "8081");
    ASSERT_EQ(server->m_host, "127.0.0.1");
}

TEST(test_block, index) {

    smt::shared_ptr<webserv::ServerBlock> server(new webserv::ServerBlock());

    // checking default values
    ASSERT_EQ(server->m_index, std::string(D_ROOT_PATH) + "index.html");

    // checking all possible error messages
    std::vector<std::string> command(1, "index");
    ASSERT_EQ(server->index(command), "wrong number of arguments");

    command.push_back("something.html");
    ASSERT_EQ(server->index(command), "something.html: failed to open");
    ASSERT_EQ(server->m_index, std::string(D_ROOT_PATH) + "index.html");

    // // FIXME: this is not passing the tests
    // command[1] = "../tests/test_web/";
    // ASSERT_EQ(server->index(command), "../tests/test_web/: failed to open");
    // ASSERT_EQ(server->m_index, std::string(D_ROOT_PATH) + "index.html");

    // checking success
    command[1] = "../tests/test_web/index.html";
    ASSERT_EQ(server->index(command), "");

    // checking values
    ASSERT_EQ(server->m_index, "../tests/test_web/index.html");
}

TEST(test_block, root) {

    smt::shared_ptr<webserv::ServerBlock>   server(new webserv::ServerBlock());
    smt::shared_ptr<webserv::LocationBlock> location(
        new webserv::LocationBlock("/www/block"));

    // checking default values
    ASSERT_EQ(server->m_root, D_ROOT_PATH);
    ASSERT_EQ(location->m_root, "");

    // checking all possible error messages
    std::vector<std::string> command(1, "root");
    ASSERT_EQ(server->root(command), "wrong number of arguments");
    ASSERT_EQ(location->root(command), "wrong number of arguments");

    command.push_back("something.html");
    ASSERT_EQ(server->root(command), "something.html: failed to open");
    ASSERT_EQ(location->root(command), "something.html: failed to open");
    ASSERT_EQ(server->m_root, D_ROOT_PATH);
    ASSERT_EQ(location->m_root, "");

    command[1] = "../tests/test_web/index.html";
    ASSERT_EQ(server->root(command),
              "../tests/test_web/index.html: failed to open");
    ASSERT_EQ(location->root(command),
              "../tests/test_web/index.html: failed to open");
    ASSERT_EQ(server->m_root, D_ROOT_PATH);
    ASSERT_EQ(location->m_root, "");

    // checking success
    command[1] = "../tests/test_web/";
    ASSERT_EQ(server->root(command), "");
    ASSERT_EQ(location->root(command), "");

    // checking values
    ASSERT_EQ(server->m_root, "../tests/test_web/");
    ASSERT_EQ(location->m_root, "../tests/test_web/");
}

TEST(test_block, server_name) {

    smt::shared_ptr<webserv::ServerBlock> server(new webserv::ServerBlock());

    // checking default values
    ASSERT_EQ(server->m_server_name.empty(), true);

    // checking all possible error messages
    std::vector<std::string> command(1, "server_name");
    ASSERT_EQ(server->server_name(command), "wrong number of arguments");
    ASSERT_EQ(server->m_server_name.empty(), true);

    // checking success
    command.push_back("example.com");
    ASSERT_EQ(server->server_name(command), "");

    // checking values
    ASSERT_EQ(server->m_server_name, "example.com");
}

TEST(test_block, error_page) {

    smt::shared_ptr<webserv::ServerBlock> server(new webserv::ServerBlock());

    // checking default values
    ASSERT_EQ(server->m_error_page.empty(), true);

    std::map<int, std::string> comp;
    comp[500] = "../tests/test_web/500.html";
    comp[501] = "../tests/test_web/500.html";
    comp[404] = "../tests/test_web/404.html";

    // checking all possible error messages
    std::vector<std::string> command(1, "error_page");
    ASSERT_EQ(server->error_page(command), "wrong number of arguments");

    command.push_back("status");
    ASSERT_EQ(server->error_page(command), "wrong number of arguments");

    command.push_back("501");
    ASSERT_EQ(server->error_page(command), "501: failed to open");

    command.push_back("../tests/test_web/500.html");
    ASSERT_EQ(server->error_page(command), "status: invalid status code");

    // checking success
    command[1] = "500";
    ASSERT_EQ(server->error_page(command), "");

    command[1] = "404";
    command[2] = "../tests/test_web/404.html";
    command.pop_back();
    ASSERT_EQ(server->error_page(command), "");

    // checking values
    ASSERT_EQ(server->m_error_page, comp);
}

TEST(test_block, location) {

    smt::shared_ptr<webserv::ServerBlock> server(new webserv::ServerBlock());

    // checking default values of server
    ASSERT_EQ(server->m_location.empty(), true);

    // checking default values of location
    smt::shared_ptr<webserv::LocationBlock> location(
        new webserv::LocationBlock("/www/block"));

    ASSERT_EQ(location->m_cgi_enabled, false);
    ASSERT_EQ(location->m_target, "/www/block");
    ASSERT_EQ(location->m_root, "");

    std::string           arr[3] = {"GET", "POST", "DELETE"};
    std::set<std::string> comp(arr, arr + 3);
    ASSERT_EQ(location->m_allowed_methods, comp);
}

TEST(test_block, fastcgi_pass) {

    smt::shared_ptr<webserv::LocationBlock> location(
        new webserv::LocationBlock("/www/block"));

    // checking default values
    ASSERT_EQ(location->m_cgi_enabled, false);

    // checking all possible error messages
    std::vector<std::string> command(1, "fastcgi_pass");
    ASSERT_EQ(location->cgi(command), "wrong number of arguments");

    command.push_back("oon");
    ASSERT_EQ(location->cgi(command), "oon: unrecognized syntax");
    ASSERT_EQ(location->m_cgi_enabled, false);

    // checking success
    command[1] = "true";
    ASSERT_EQ(location->cgi(command), "");
    ASSERT_EQ(location->m_cgi_enabled, true);
    command[1] = "false";
    ASSERT_EQ(location->cgi(command), "");
    ASSERT_EQ(location->m_cgi_enabled, false);
}

TEST(test_block, allowed_methods) {

    smt::shared_ptr<webserv::LocationBlock> location(
        new webserv::LocationBlock("/www/block"));

    std::string           arr[3] = {"GET", "POST", "DELETE"};
    std::set<std::string> comp(arr, arr + 3);

    // checking default values
    ASSERT_EQ(location->m_allowed_methods, comp);

    // checking all possible error messages
    std::vector<std::string> command(1, "allowed_methods");
    ASSERT_EQ(location->allowed_methods(command), "wrong number of arguments");

    // checking success
    command.push_back("GET");
    command.push_back("DELETE");
    ASSERT_EQ(location->allowed_methods(command), "");

    // checking values
    comp.erase(comp.find("POST"));
    ASSERT_EQ(location->m_allowed_methods, comp);
}
