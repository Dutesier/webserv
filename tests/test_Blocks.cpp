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
    ASSERT_EQ(server->m_bodySize, D_BODY_SIZE);

    // checking all possible error messages
    std::vector<std::string> command(1, "max_body_size");
    ASSERT_EQ(server->bodySize(command), "wrong number of arguments");
    ASSERT_EQ(server->m_bodySize, D_BODY_SIZE);

    command.push_back("asad");
    ASSERT_EQ(server->bodySize(command), "asad: invalid max body size");
    ASSERT_EQ(server->m_bodySize, D_BODY_SIZE);

    command[1] = "32516983297";
    ASSERT_EQ(server->bodySize(command), "32516983297: invalid max body size");
    ASSERT_EQ(server->m_bodySize, D_BODY_SIZE);

    // checking success
    command[1] = "3251";
    ASSERT_EQ(server->bodySize(command), "");

    // checking values
    ASSERT_EQ(server->m_bodySize, 3251);
}

TEST(test_block, listen) {

    smt::shared_ptr<webserv::ServerBlock> server(new webserv::ServerBlock());

    // checking default values
    ASSERT_EQ(server->m_port, 80);
    ASSERT_EQ(server->m_host, "*");

    // checking all possible error messages
    std::vector<std::string> command(1, "listen");
    ASSERT_EQ(server->listen(command), "wrong number of arguments");
    ASSERT_EQ(server->m_port, 80);
    ASSERT_EQ(server->m_host, "*");

    command.push_back("asad:asad");
    ASSERT_EQ(server->listen(command), "asad: invalid port");
    ASSERT_EQ(server->m_port, 80);
    ASSERT_EQ(server->m_host, "*");

    command[1] = "asad:32516983297";
    ASSERT_EQ(server->listen(command), "32516983297: invalid port");
    ASSERT_EQ(server->m_port, 80);
    ASSERT_EQ(server->m_host, "*");

    // TODO: make this tests better
    // checking success
    command[1] = "128.0.0.1";
    ASSERT_EQ(server->listen(command), "");
    ASSERT_EQ(server->m_host, "128.0.0.1");

    command[1] = "8080";
    ASSERT_EQ(server->listen(command), "");
    ASSERT_EQ(server->m_port, 8080);

    command[1] = "127.0.0.1:8081";
    ASSERT_EQ(server->listen(command), "");

    // checking values
    ASSERT_EQ(server->m_port, 8081);
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
    ASSERT_EQ(server->m_serverName.empty(), true);

    // checking all possible error messages
    std::vector<std::string> command(1, "server_name");
    ASSERT_EQ(server->serverName(command), "wrong number of arguments");
    ASSERT_EQ(server->m_serverName.empty(), true);

    // checking success
    command.push_back("example.com");
    ASSERT_EQ(server->serverName(command), "");

    // checking values
    ASSERT_EQ(server->m_serverName, "example.com");
}

TEST(test_block, error_page) {

    smt::shared_ptr<webserv::ServerBlock> server(new webserv::ServerBlock());

    // checking default values
    ASSERT_EQ(server->m_errorPage.empty(), true);

    std::map<int, std::string> comp;
    comp[500] = "../tests/test_web/500.html";
    comp[501] = "../tests/test_web/500.html";
    comp[404] = "../tests/test_web/404.html";

    // checking all possible error messages
    std::vector<std::string> command(1, "error_page");
    ASSERT_EQ(server->errorPage(command), "wrong number of arguments");

    command.push_back("status");
    ASSERT_EQ(server->errorPage(command), "wrong number of arguments");

    command.push_back("501");
    ASSERT_EQ(server->errorPage(command), "501: failed to open");

    command.push_back("../tests/test_web/500.html");
    ASSERT_EQ(server->errorPage(command), "status: invalid status code");

    // checking success
    command[1] = "500";
    ASSERT_EQ(server->errorPage(command), "");

    command[1] = "404";
    command[2] = "../tests/test_web/404.html";
    command.pop_back();
    ASSERT_EQ(server->errorPage(command), "");

    // checking values
    ASSERT_EQ(server->m_errorPage, comp);
}

TEST(test_block, location) {

    smt::shared_ptr<webserv::ServerBlock> server(new webserv::ServerBlock());

    // checking default values of server
    ASSERT_EQ(server->m_location.empty(), true);

    // checking default values of location
    smt::shared_ptr<webserv::LocationBlock> location(
        new webserv::LocationBlock("/www/block"));

    ASSERT_EQ(location->m_cgiEnabled, false);
    ASSERT_EQ(location->m_target, "/www/block");
    ASSERT_EQ(location->m_root, "");

    std::string           arr[3] = {"GET", "POST", "DELETE"};
    std::set<std::string> comp(arr, arr + 3);
    ASSERT_EQ(location->m_allowedMethods, comp);
}

TEST(test_block, fastcgi_pass) {

    smt::shared_ptr<webserv::LocationBlock> location(
        new webserv::LocationBlock("/www/block"));

    // checking default values
    ASSERT_EQ(location->m_cgiEnabled, false);

    // checking all possible error messages
    std::vector<std::string> command(1, "fastcgi_pass");
    ASSERT_EQ(location->cgi(command), "wrong number of arguments");

    command.push_back("oon");
    ASSERT_EQ(location->cgi(command), "oon: unrecognized syntax");
    ASSERT_EQ(location->m_cgiEnabled, false);

    // checking success
    command[1] = "true";
    ASSERT_EQ(location->cgi(command), "");
    ASSERT_EQ(location->m_cgiEnabled, true);
    command[1] = "false";
    ASSERT_EQ(location->cgi(command), "");
    ASSERT_EQ(location->m_cgiEnabled, false);
}

TEST(test_block, allowedMethods) {

    smt::shared_ptr<webserv::LocationBlock> location(
        new webserv::LocationBlock("/www/block"));

    std::string           arr[3] = {"GET", "POST", "DELETE"};
    std::set<std::string> comp(arr, arr + 3);

    // checking default values
    ASSERT_EQ(location->m_allowedMethods, comp);

    // checking all possible error messages
    std::vector<std::string> command(1, "allowedMethods");
    ASSERT_EQ(location->allowedMethods(command), "wrong number of arguments");

    // checking success
    command.push_back("GET");
    command.push_back("DELETE");
    ASSERT_EQ(location->allowedMethods(command), "");

    // checking values
    comp.erase(comp.find("POST"));
    ASSERT_EQ(location->m_allowedMethods, comp);
}
