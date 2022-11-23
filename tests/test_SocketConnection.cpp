#include "Client.hpp"
#include "Socket.hpp"
#include "SocketAddress.hpp"
#include "SocketConnection.hpp"
#include "TCPSocket.hpp"

#include <cstdio>
#include <gtest/gtest.h>

#define HTTP_REQ     "GET / HTTP/1.1\r\nHost:x\r\n\r\n"
#define HTTP_REQ_LEN 27
#define HTTP_RES     "HTTP/1.1 404\r\nContent-Length: 0\r\n"
#define HTTP_RES_LEN 34

class test_SocketConnection : public ::testing::Test {

    public:

        void SetUp(void) {

            this->sock = new webserv::TCPSocket(8080);
            // setting socket options
            const int e = 1;
            this->sock->setsockopt(SOL_SOCKET, SO_REUSEADDR, &e, sizeof(int));
            this->sock->setsockopt(SOL_SOCKET, SO_REUSEPORT, &e, sizeof(int));
            struct timeval timeout;
            timeout.tv_sec = 10;
            timeout.tv_usec = 0;
            this->sock->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout,
                                   sizeof(struct timeval));
            // preparing socket for requests
            this->sock->bind();
            this->sock->listen();
            // preparing client and connection
            this->client = new Client(8080);
            this->sock->accept();
            auto v = this->sock->get_connections();
            if (v.empty()) this->connection = nullptr;
            this->connection = v[0];
        }

        void TearDown(void) {
            delete this->sock;
            delete this->client;
        }

    protected:

        Client*                    client;
        webserv::TCPSocket*        sock;
        webserv::SocketConnection* connection;
};

TEST_F(test_SocketConnection, constructor) {
    ASSERT_NE(this->connection, nullptr);
}

TEST_F(test_SocketConnection, destructor) {
    ASSERT_NE(this->connection, nullptr);
}

TEST_F(test_SocketConnection, close) {
    ASSERT_NE(this->connection, nullptr);
    ASSERT_TRUE(this->connection->close()) << errno;
    ASSERT_EQ(this->connection->sockfd(), -1);
    std::string message = "HTTP/1.1 404\r\nContent-Length: 0\r\n";
    ASSERT_FALSE(this->connection->send(message)) << errno;
}

TEST_F(test_SocketConnection, recv) {
    ASSERT_NE(this->connection, nullptr);
    this->client->send_message(HTTP_REQ);
    auto str = this->connection->recv();
    ASSERT_NE(str, "");
}

TEST_F(test_SocketConnection, send) {
    ASSERT_NE(this->connection, nullptr);
    ASSERT_TRUE(this->connection->send(HTTP_RES)) << errno;
    ASSERT_STREQ(client->receive_message().c_str(), HTTP_RES) << errno;
}
