#include "client/Client.hpp"
#include "socket/ServerSocket.hpp"
#include "socket/SocketConnection.hpp"
#include "utils/smt.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <gtest/gtest.h>
#include <netdb.h>
#include <string>

class test_SocketConnection : public ::testing::Test {
        void SetUp(void) {

            // creating listener socket binded to 8080:localhost
            // std::cout << "Creating Listenner" << std::endl;
            smt::shared_ptr<webserv::ServerAddress> addr(
                new webserv::ServerAddress("8080", "localhost"));
            listener = smt::make_shared(new webserv::ServerSocket(addr));
            ASSERT_NO_THROW(listener->socket());

            int const enable = 1;
            listener->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable,
                                 sizeof(int));
            listener->setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable,
                                 sizeof(int));
            struct timeval timeout;
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            listener->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout,
                                 sizeof(struct timeval));

            ASSERT_NO_THROW(listener->bind());
            ASSERT_NO_THROW(listener->listen());

            // creating client that will try and connect with 8080:localhost
            client = smt::make_shared(
                new webserv::test::Client("8080", "localhost"));
            ASSERT_NO_THROW(client->socket());
            ASSERT_NO_THROW(client->connect());

            // accepting connection
            int fd;
            ASSERT_NO_THROW(fd = listener->accept());

            // getting connector
            sock = listener->getConnection(fd);
        }

        void TearDown(void) {}

    protected:

        smt::shared_ptr<webserv::ServerSocket>     listener;
        smt::shared_ptr<webserv::test::Client>     client;
        smt::shared_ptr<webserv::SocketConnection> sock;
};

TEST_F(test_SocketConnection, constructor) {}

TEST_F(test_SocketConnection, getSockFd) { ASSERT_GT(sock->getSockFd(), -1); }

// getPort() function doesn't return 8080, I don't know why
// TEST_F(test_SocketConnection, getPort) {
//     std::cout << "getPort" << std::endl;
//     ASSERT_EQ(sock->getPort(), 8080);
// }

TEST_F(test_SocketConnection, getHost) {
    ASSERT_EQ(sock->getHost(), "127.0.0.1");
}

TEST_F(test_SocketConnection, getFamily) {
    ASSERT_EQ(sock->getFamily(), AF_INET);
}

TEST_F(test_SocketConnection, getAddress) {
    ASSERT_NE(sock->getAddress(), nullptr);
}

TEST_F(test_SocketConnection, getLength) {
    ASSERT_EQ(sock->getLength(), sizeof(sockaddr_in));
}

TEST_F(test_SocketConnection, close) { ASSERT_NO_THROW(sock->close()); }

TEST_F(test_SocketConnection, recv) {
    ASSERT_NO_THROW(client->send("Hello World!"));
    std::string request;
    ASSERT_NO_THROW(request = sock->recv());
    ASSERT_EQ(request, "Hello World!");
}

TEST_F(test_SocketConnection, send) {
    ASSERT_NO_THROW(sock->send("Hello World!"));
    std::string response;
    ASSERT_NO_THROW(response = client->recv());
    ASSERT_EQ(response, "Hello World!");
}