#include "socket/SocketAddress.hpp"

#include <gtest/gtest.h>

TEST(test_Address, constructor) {

    smt::shared_ptr<webserv::SocketAddress> addr;

    ASSERT_NO_THROW(addr = smt::make_shared(new webserv::SocketAddress(80, "127.0.0.1")));
    ASSERT_THROW(addr = smt::make_shared(new webserv::SocketAddress(80, "localhost")),
                 webserv::SocketAddress::InvalidIPAddressException);
}

TEST(test_Address, destructor) {

    webserv::SocketAddress* addr;

    ASSERT_NO_THROW(addr = new webserv::SocketAddress(80, "127.0.0.1"));
    ASSERT_NO_THROW(delete addr);
}

TEST(test_Address, address) {

    smt::shared_ptr<webserv::SocketAddress> addr(new webserv::SocketAddress(8000, "*"));

    struct sockaddr* a = addr->address();

    ASSERT_EQ(typeid(a), typeid(struct sockaddr*));
    ASSERT_NE(typeid(a), typeid(struct sockaddr_in*));
    ASSERT_EQ(a->sa_family, addr->family());
    ASSERT_EQ(reinterpret_cast<struct sockaddr_in*>(a)->sin_port,
              htons(addr->port()));
    ASSERT_EQ(reinterpret_cast<struct sockaddr_in*>(a)->sin_addr.s_addr,
              INADDR_ANY);
}

TEST(test_Address, length) {

    smt::shared_ptr<webserv::SocketAddress> addr(new webserv::SocketAddress(8000));
    struct sockaddr*              a = addr->address();

    ASSERT_EQ(typeid(addr->length()), typeid(socklen_t));
    ASSERT_EQ(addr->length(),
              sizeof(*(reinterpret_cast<struct sockaddr_in*>(a))));
    ASSERT_EQ(addr->length(), sizeof(*a));
}

TEST(test_Address, port) {

    smt::shared_ptr<webserv::SocketAddress> addr(new webserv::SocketAddress(8000));

    ASSERT_EQ(addr->port(), 8000);
    ASSERT_EQ(reinterpret_cast<struct sockaddr_in*>(addr->address())->sin_port,
              htons(addr->port()));
}

TEST(test_Address, host) {

    smt::shared_ptr<webserv::SocketAddress> addr(new webserv::SocketAddress(8000, "128.0.0.1"));

    ASSERT_EQ(addr->host(), "128.0.0.1");
}

TEST(test_Address, family) {

    smt::shared_ptr<webserv::SocketAddress> addr(
        new webserv::SocketAddress(8000, "128.0.0.1", AF_INET6));

    ASSERT_EQ(addr->family(), AF_INET6);
}
