#include "client/Client.hpp"
#include "socket/ServerSocket.hpp"
#include "utils/smt.hpp"

#include <gtest/gtest.h>

TEST(test_ServerSocket, constructor) {

    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    ASSERT_NO_THROW(smt::make_shared(new webserv::ServerSocket(addr)));

    addr = smt::make_shared(new webserv::ServerAddress("8080", "127.0.0.1"));
    ASSERT_NO_THROW(smt::make_shared(new webserv::ServerSocket(addr)));
}

TEST(test_ServerSocket, getSockFd) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    smt::shared_ptr<webserv::ServerSocket> sock(
        new webserv::ServerSocket(addr));
    ASSERT_EQ(sock->getSockFd(), -1);
    sock->socket();
    ASSERT_GT(sock->getSockFd(), -1);
}

TEST(test_ServerSocket, getHost) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    smt::shared_ptr<webserv::ServerSocket> sock(
        new webserv::ServerSocket(addr));
    ASSERT_EQ(sock->getHost(), "127.0.0.1");
}

TEST(test_ServerSocket, getPort) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    smt::shared_ptr<webserv::ServerSocket> sock(
        new webserv::ServerSocket(addr));
    ASSERT_EQ(sock->getPort(), 8080);
}

TEST(test_ServerSocket, getFamily) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    smt::shared_ptr<webserv::ServerSocket> sock(
        new webserv::ServerSocket(addr));
    ASSERT_EQ(sock->getFamily(), AF_INET);
}

TEST(test_ServerSocket, getType) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    smt::shared_ptr<webserv::ServerSocket> sock(
        new webserv::ServerSocket(addr));
    ASSERT_EQ(sock->getType(), SOCK_STREAM);
}

TEST(test_ServerSocket, getAddress) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    smt::shared_ptr<webserv::ServerSocket> sock(
        new webserv::ServerSocket(addr));
    sockaddr_in* ptrAddr = reinterpret_cast<sockaddr_in*>(sock->getAddress());
    ASSERT_EQ("127.0.0.1", sock->getHost());
    ASSERT_EQ(ptrAddr->sin_port, htons(sock->getPort()));
    ASSERT_EQ(ptrAddr->sin_family, sock->getFamily());
    ASSERT_EQ(SOCK_STREAM, sock->getType());
}

void setOptions(smt::shared_ptr<webserv::ServerSocket> sock) {

    int const enable = 1;
    sock->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    sock->setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    sock->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));
}

TEST(test_ServerSocket, socket) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    smt::shared_ptr<webserv::ServerSocket> sock(
        new webserv::ServerSocket(addr));
    ASSERT_EQ(sock->getSockFd(), -1);
    ASSERT_NO_THROW(sock->socket());
    ASSERT_GT(sock->getSockFd(), -1);
}

TEST(test_ServerSocket, bind) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    smt::shared_ptr<webserv::ServerSocket> sock(
        new webserv::ServerSocket(addr));
    ASSERT_NO_THROW(sock->socket());
    ASSERT_NO_THROW(setOptions(sock)) << errno;
    ASSERT_NO_THROW(sock->bind());
}

TEST(test_ServerSocket, listen) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    smt::shared_ptr<webserv::ServerSocket> sock(
        new webserv::ServerSocket(addr));
    ASSERT_NO_THROW(sock->socket());
    ASSERT_NO_THROW(setOptions(sock)) << errno;
    ASSERT_NO_THROW(sock->bind());
    ASSERT_NO_THROW(sock->listen());
}

TEST(test_ServerSocket, accept) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    smt::shared_ptr<webserv::ServerSocket> sock(
        new webserv::ServerSocket(addr));
    ASSERT_NO_THROW(sock->socket());
    ASSERT_NO_THROW(setOptions(sock)) << errno;
    ASSERT_NO_THROW(sock->bind());
    ASSERT_NO_THROW(sock->listen());
    ASSERT_EQ(sock->getHost(), "127.0.0.1");
    ASSERT_EQ(sock->getPort(), 8080);

    // creating client that will try and connect with 8080:localhost
    smt::shared_ptr<webserv::test::Client> client(
        new webserv::test::Client("8080", "localhost"));
    ASSERT_NO_THROW(client->socket());
    ASSERT_NO_THROW(client->connect());

    int connectFd;
    ASSERT_NO_THROW(connectFd = sock->accept());
    ASSERT_GT(connectFd, -1);
}

// TEST(test_ServerSocket, close) {
//     smt::shared_ptr<webserv::ServerAddress> addr(
//         new webserv::ServerAddress("8080", "localhost"));
//     smt::shared_ptr<webserv::ServerSocket> sock(
//         new webserv::ServerSocket(addr));
//     ASSERT_NO_THROW(sock->socket());
//     ASSERT_NO_THROW(setOptions(sock)) << errno;
//     ASSERT_NO_THROW(sock->bind());
//     ASSERT_NO_THROW(sock->close());
// }

// TEST(test_ServerSocket, recv) {
//     smt::shared_ptr<webserv::ServerAddress> addr(
//         new webserv::ServerAddress("8080", "localhost"));
//     smt::shared_ptr<webserv::ServerSocket> sock(
//         new webserv::ServerSocket(addr));
//     ASSERT_NO_THROW(sock->socket());
//     ASSERT_NO_THROW(setOptions(sock)) << errno;
//     ASSERT_NO_THROW(sock->bind());
//     ASSERT_NO_THROW(sock->listen());

// webserv::test::Client client("8080", "localhost");
// ASSERT_NO_THROW(client.socket());
// ASSERT_NO_THROW(client.connect());
// ASSERT_NO_THROW(client.send("Hello World!"));

// int connectFd;
// ASSERT_NO_THROW(connectFd = sock->accept());
// ASSERT_GT(connectFd, -1);
// std::string request;
// ASSERT_NO_THROW(request = sock->recv(connectFd));
// ASSERT_EQ(request, "Hello World!");
// }

// TEST(test_ServerSocket, send) {
//     smt::shared_ptr<webserv::ServerAddress> addr(
//         new webserv::ServerAddress("8080", "localhost"));
//     smt::shared_ptr<webserv::ServerSocket> sock(
//         new webserv::ServerSocket(addr));
//     ASSERT_NO_THROW(sock->socket());
//     ASSERT_NO_THROW(setOptions(sock)) << errno;
//     ASSERT_NO_THROW(sock->bind());
//     ASSERT_NO_THROW(sock->listen());

// smt::shared_ptr<webserv::test::Client> client(
//     new webserv::test::Client("8080", "localhost"));
// ASSERT_NO_THROW(client->socket());
// ASSERT_NO_THROW(client->connect());
// ASSERT_NO_THROW(client->send("Hello World!"));

// int connectFd;
// ASSERT_NO_THROW(connectFd = sock->accept());
// ASSERT_GT(connectFd, -1);
// std::string request;
// ASSERT_NO_THROW(request = sock->recv(connectFd));
// ASSERT_EQ(request, "Hello World!");
// ASSERT_NO_THROW(sock->send(connectFd, request));
// std::string response;
// ASSERT_NO_THROW(response = client->recv());
// ASSERT_EQ(response, "Hello World!");
// }