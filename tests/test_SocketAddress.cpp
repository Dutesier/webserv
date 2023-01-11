
#include "socket/Socket.hpp"
#include "socket/SocketAddress.hpp"
#include "socket/TCPSocket.hpp"

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
