
#include "Client.hpp"
#include "TCPSocket.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <gtest/gtest.h>
#include <thread>

#define HTTP_REQ     "GET / HTTP/1.1\r\nHost:x\r\n\r\n"
#define HTTP_REQ_LEN 27
#define HTTP_RES     "HTTP/1.1 404\r\nContent-Length: 0\r\n"
#define HTTP_RES_LEN 34

class test_activeConnection : public ::testing::Test {
    public:

        void SetUp() {
            // This runs on TEST_F start
            listener = new webserv::TCPSocket(8042, "localhost", AF_INET);
            set_options();
            listener->bind();
            listener->listen();

            client = new Client(8042);
            listener->accept();

            // NOTE: on setup a functional TCP connection is created.
        }

        void TearDown() {
            // This runs on TEST_F end
            delete client;
            std::cout << "OK" << std::endl;
            delete listener;
        }

        void set_options(void) {
            const int      enable = 1;
            struct timeval timeout;
            timeout.tv_sec = 10;
            timeout.tv_usec = 0;
            this->listener->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable,
                                       sizeof(int));
            this->listener->setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable,
                                       sizeof(int));
            this->listener->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout,
                                       sizeof(struct timeval));
        }

    protected:

        webserv::TCPSocket* listener;
        Client*             client;
};

// The following tests test an already established connection between a listener
// and a client
TEST_F(test_activeConnection, CanReadFromConnection) {
    // ASSERT_NO_THROW(client->send_message(HTTP_REQ));
    //
    // ASSERT_STREQ(HTTP_REQ, listener->recv(NULL).c_str());
}

TEST_F(test_activeConnection, CanWriteToConnection) {
    // ASSERT_NO_THROW(listener->send(NULL, HTTP_RES));
    //
    // ASSERT_STREQ(HTTP_RES, client->receive_message().c_str());
}
