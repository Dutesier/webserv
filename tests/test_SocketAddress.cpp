
#include "Socket.hpp"
#include "SocketAddress.hpp"
#include "TCPSocket.hpp"

#include <gtest/gtest.h>

class test_SocketAddress : public ::testing::Test {

    public:

        void SetUp(void) {
            // this->addr = new webserv::SocketAddress(443);
            this->sock = new webserv::TCPSocket(443);
            this->addr = this->sock->address();
        }

        void TearDown(void) { delete this->sock; }

    protected:

        webserv::TCPSocket*    sock;
        webserv::SocketAddress addr;
};

TEST_F(test_SocketAddress, default_constructor) {}

TEST_F(test_SocketAddress, copy_constructor) {
    webserv::SocketAddress* a = new webserv::SocketAddress(this->addr);
    // webserv::SocketAddress*	b;
    // *b = *a;

    // ASSERT_EQ(a->address(), this->addr.address());
    // ASSERT_EQ(b->address(), this->addr.address());
    //
    // ASSERT_EQ(a->length(), this->addr.length());
    // ASSERT_EQ(b->length(), this->addr.length());
    //
    // ASSERT_EQ(a->host(), this->addr.host());
    // ASSERT_EQ(b->host(), this->addr.host());
    //
    // ASSERT_EQ(a->port(), this->addr.port());
    // ASSERT_EQ(b->port(), this->addr.port());
    //
    // ASSERT_EQ(a->family(), this->addr.family());
    // ASSERT_EQ(b->family(), this->addr.family());
    delete a;
}

TEST_F(test_SocketAddress, destructor) {}

TEST_F(test_SocketAddress, address) {

    struct sockaddr* a = this->addr.address();
    ASSERT_EQ(typeid(a), typeid(struct sockaddr*));
    ASSERT_NE(typeid(a), typeid(struct sockaddr_in*));
    ASSERT_EQ(a->sa_family, this->addr.family());
    ASSERT_EQ(reinterpret_cast<struct sockaddr_in*>(a)->sin_port,
              htons(this->addr.port()));
    ASSERT_EQ(reinterpret_cast<struct sockaddr_in*>(a)->sin_addr.s_addr,
              INADDR_ANY);
}

TEST_F(test_SocketAddress, length) {

    struct sockaddr* a = this->addr.address();
    ASSERT_EQ(typeid(this->addr.length()), typeid(socklen_t));
    ASSERT_EQ(this->addr.length(),
              sizeof(*(reinterpret_cast<struct sockaddr_in*>(a))));
    ASSERT_EQ(this->addr.length(), sizeof(*a));
}

TEST_F(test_SocketAddress, port) {

    ASSERT_EQ(this->addr.port(), 443);

    webserv::SocketAddress* a = new webserv::SocketAddress(80);
    webserv::SocketAddress* b = new webserv::SocketAddress(8080);

    ASSERT_EQ(a->port(), 80);
    ASSERT_EQ(b->port(), 8080);

    ASSERT_EQ(reinterpret_cast<struct sockaddr_in*>(a->address())->sin_port,
              htons(a->port()));
    ASSERT_EQ(reinterpret_cast<struct sockaddr_in*>(b->address())->sin_port,
              htons(b->port()));

    delete a;
    delete b;
}

TEST_F(test_SocketAddress, host) {

    ASSERT_EQ(this->addr.host(), "localhost");

    webserv::SocketAddress* a = new webserv::SocketAddress(80, "example.com");
    webserv::SocketAddress* b =
        new webserv::SocketAddress(8080, "www.example.com");

    ASSERT_EQ(a->host(), "example.com");
    ASSERT_EQ(b->host(), "www.example.com");

    delete a;
    delete b;
}

TEST_F(test_SocketAddress, family) {

    ASSERT_EQ(this->addr.family(), AF_INET);

    webserv::SocketAddress* a =
        new webserv::SocketAddress(80, "localhost", AF_INET6);
    ASSERT_EQ(a->family(), AF_INET6);

    delete a;
}

TEST_F(test_SocketAddress, has_ipv4) {

    ASSERT_TRUE(this->addr.has_ipv4());
    ASSERT_FALSE(this->addr.has_ipv6());

    webserv::SocketAddress* a =
        new webserv::SocketAddress(80, "localhost", AF_INET6);
    ASSERT_TRUE(a->has_ipv4());
    ASSERT_TRUE(a->has_ipv6());

    delete a;
}
