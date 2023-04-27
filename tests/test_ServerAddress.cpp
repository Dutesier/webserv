#include "socket/ServerAddress.hpp"

#include <gtest/gtest.h>

TEST(test_ServerAddress, constructor) {
    ASSERT_NO_THROW(
        smt::make_shared(new webserv::ServerAddress("8080", "localhost")));
    ASSERT_NO_THROW(
        smt::make_shared(new webserv::ServerAddress("8080", "127.0.0.1")));
    ASSERT_NO_THROW(
        smt::make_shared(new webserv::ServerAddress("8081", "localhost")));

    ASSERT_THROW(smt::make_shared(
                     new webserv::ServerAddress("invalid-port", "localhost")),
                 webserv::ServerAddress::GetAddInfoFailureException);

    ASSERT_THROW(
        smt::make_shared(new webserv::ServerAddress("8080", "invalid-host")),
        webserv::ServerAddress::GetAddInfoFailureException);
}

TEST(test_ServerAddress, getHost) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    ASSERT_EQ(addr->getHost(), "127.0.0.1");
}

TEST(test_ServerAddress, getPort) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    ASSERT_EQ(addr->getPort(), 8080);
}

TEST(test_ServerAddress, getFamily) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    ASSERT_EQ(addr->getFamily(), AF_INET);
}

TEST(test_ServerAddress, getType) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    ASSERT_EQ(addr->getType(), SOCK_STREAM);
}

TEST(test_ServerAddress, getAddress) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    sockaddr_in* ptrAddr = reinterpret_cast<sockaddr_in*>(addr->getAddress());
    ASSERT_EQ("127.0.0.1", addr->getHost());
    ASSERT_EQ(ptrAddr->sin_port, htons(addr->getPort()));
    ASSERT_EQ(ptrAddr->sin_family, addr->getFamily());
    ASSERT_EQ(SOCK_STREAM, addr->getType());
}

TEST(test_ServerAddress, getLength) {
    smt::shared_ptr<webserv::ServerAddress> addr(
        new webserv::ServerAddress("8080", "localhost"));
    ASSERT_EQ(addr->getLength(), sizeof(sockaddr_in));
}