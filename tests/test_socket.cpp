#include "Client.hpp"
#include "socket/ServerSocket.hpp"
#include "socket/SocketConnection.hpp"
#include "socket/TCPSocket.hpp"

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
            int const      enable = 1;
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
// TEST_F(test_activeConnection, CanReadFromConnection) {
//     EXPECT_TRUE(client->send_message(HTTP_REQ));

// ASSERT_STREQ(HTTP_REQ, listener->recv(NULL).c_str());
// }

// TEST_F(test_activeConnection, CanWriteToConnection) {
//     EXPECT_TRUE(listener->send(NULL, HTTP_RES));

// ASSERT_STREQ(HTTP_RES, client->receive_message().c_str());
// }

TEST(test_socket, recv_many_req) {

    std::string req0 =
        "HTTP/1.1 404\r\nContent-Length: "
        "120\r\n\r\n..........................................................."
        "............................................................";

    std::string req1 =
        "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nHost: "
        "www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n";

    std::string req2 = "POST /cgi-bin/process.cgi HTTP/1.1\r\nUser-Agent: "
                       "Mozilla/4.0 (compatible; MSIE5.01; Windows "
                       "NT)\r\nHost: www.tutorialspoint.com\r\nContent-Type: "
                       "text/xml; charset=utf-8\r\nContent-Length: "
                       "95\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, "
                       "deflate\r\nConnection: Keep-Alive\r\n\r\n<?xml "
                       "version=\"1.0\" encoding=\"utf-8\"?>\r\n<string "
                       "xmlns=\"http://clearforest.com/\">string</string>";

    std::string req3 =
        "GET /hello.htm HTTP/1.0\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\nConnection: Keep-Alive\r\n\r\n";

    webserv::ServerSocket tmp(8080);
    Client                client(8080);
    int                   fd;

    ASSERT_NO_THROW(fd = tmp.accept()) << errno;
    auto it = tmp.m_connection.find(fd);
    ASSERT_NE(it, tmp.m_connection.end());
    smt::shared_ptr<webserv::SocketConnection> sock = (*it).second;

    client.send_message(req0);
    ASSERT_EQ(sock->recv(), req0);

    client.send_message(req1);
    ASSERT_EQ(sock->recv(), req1);

    client.send_message(req2);
    client.send_message(req3);
    ASSERT_EQ(sock->recv(), req2 + req3);
}

TEST(test_socket, get_request_imcomplete) {

    std::string req1 = "POST /cgi-bin/process.cgi HTTP/1.1\r\nUser-Agent: "
                       "Mozilla/4.0 (compatible; MSIE5.01; Windows "
                       "NT)\r\nHost: www.tutorialspoint.com\r\nContent-Type: "
                       "text/xml; charset=utf-8\r\n";

    std::string req2 =
        "Content-Length: 95\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n<?xml version=\"1.0\" "
        "encoding=\"utf-8\"?>\r\n<string "
        "xmlns=\"http://clearforest.com/\">string</string>";

    webserv::ServerSocket tmp(8080);
    Client                client(8080);
    int                   fd;

    ASSERT_NO_THROW(fd = tmp.accept()) << errno;
    auto it = tmp.m_connection.find(fd);
    ASSERT_NE(it, tmp.m_connection.end());
    smt::shared_ptr<webserv::SocketConnection> sock = (*it).second;

    // receiving req1 - incomplete
    ASSERT_EQ(sock->getNextRequest(req1), "");
    // receiving req2 - completes req1
    ASSERT_EQ(sock->getNextRequest(req2), req1 + req2);
    // checking if there's any request left
    ASSERT_EQ(sock->getNextRequest(), "");
}

TEST(test_socket, get_request_more_than_one) {

    std::string req1 =
        "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nHost: "
        "www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n";

    std::string req2 =
        "GET /hello.htm HTTP/1.0\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\nConnection: Keep-Alive\r\n\r\n";

    webserv::ServerSocket tmp(8080);
    Client                client(8080);
    int                   fd;

    ASSERT_NO_THROW(fd = tmp.accept()) << errno;
    auto it = tmp.m_connection.find(fd);
    ASSERT_NE(it, tmp.m_connection.end());
    smt::shared_ptr<webserv::SocketConnection> sock = (*it).second;

    // receiving req1 - req2 stays in buffer
    ASSERT_EQ(sock->getNextRequest(req1 + req2), req1);
    // receiving req2 - buffer is empty
    ASSERT_EQ(sock->getNextRequest(), req2);
    // checking if there's any request left
    ASSERT_EQ(sock->getNextRequest(), "");
}

TEST(test_socket, get_next_request_extreme) {

    std::string req1 =
        "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nHost: "
        "www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n";

    std::string req2 =
        "GET /hello.htm HTTP/1.0\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\nConnection: Keep-Alive\r\n\r\n";

    std::string req3 = "POST /cgi-bin/process.cgi HTTP/1.1\r\nUser-Agent: "
                       "Mozilla/4.0 (compatible; MSIE5.01; Windows "
                       "NT)\r\nHost: www.tutorialspoint.com\r\nContent-Type: "
                       "text/xml; charset=utf-8\r\n";

    std::string req4 =
        "Content-Length: 95\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n<?xml version=\"1.0\" "
        "encoding=\"utf-8\"?>\r\n<string "
        "xmlns=\"http://clearforest.com/\">string</string>";

    std::string req5 =
        "GET /hello.htm HTTP/1.0\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\nConnection: Keep-Alive\r\n\r\n";

    webserv::ServerSocket tmp(8080);
    Client                client(8080);
    int                   fd;

    ASSERT_NO_THROW(fd = tmp.accept()) << errno;
    auto it = tmp.m_connection.find(fd);
    ASSERT_NE(it, tmp.m_connection.end());
    smt::shared_ptr<webserv::SocketConnection> sock = (*it).second;

    // receiving req1 - req2 is complete and stays in buffer, req3 is incomplete
    // and also stays in buffer
    ASSERT_EQ(sock->getNextRequest(req1 + req2 + req3), req1);
    // receiving req2 - buffer stays with req3 - incomplete
    ASSERT_EQ(sock->getNextRequest(), req2);
    // checking if there's any request left
    ASSERT_EQ(sock->getNextRequest(), "");
    // receiving req3 + req4 - buffer stays with req5
    ASSERT_EQ(sock->getNextRequest(req4 + req5), req3 + req4);
    // receiving req5 - buffer is clean
    ASSERT_EQ(sock->getNextRequest(), req5);
    // checking if there's any request left
    ASSERT_EQ(sock->getNextRequest(), "");
}
