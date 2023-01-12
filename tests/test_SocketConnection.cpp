#include "Client.hpp"
#include "socket/Socket.hpp"
#include "socket/SocketAddress.hpp"
#include "socket/SocketConnection.hpp"
#include "socket/TCPSocket.hpp"

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
            int const e = 1;
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
            int fd = this->sock->accept();
            this->connection = this->sock->m_connection[fd];
        }

        void TearDown(void) {}

    protected:

        smt::shared_ptr<Client>                    client;
        smt::shared_ptr<webserv::TCPSocket>        sock;
        smt::shared_ptr<webserv::SocketConnection> connection;
};

TEST_F(test_SocketConnection, constructor) {}

TEST_F(test_SocketConnection, destructor) {}

TEST_F(test_SocketConnection, close) {

    ASSERT_NO_THROW(this->connection->close()) << errno;
    ASSERT_EQ(this->connection->sockfd(), -1);
    std::string message = "HTTP/1.1 404\r\nContent-Length: 0\r\n";
    ASSERT_THROW(this->connection->send(message),
                 webserv::SocketConnection::SendFailureException)
        << errno;
}

TEST_F(test_SocketConnection, recv) {

    this->client->send_message(HTTP_REQ);
    std::string str = this->connection->recv();
    ASSERT_NE(str, "");
}

// TEST_F(test_SocketConnection, hardcore) {
//     ASSERT_NE(this->connection, nullptr);
//     this->client->send_message("GET /usr/home
//     HTTP/1.1\r\nHost:x\r\nContent-Length:
//     120\r\n\r\n........................................................................................................................");

// auto httpRequest = this->connection->recv();
// ASSERT_NE(httpRequest, NULL);
// ASSERT_EQ(httpRequest->getMethod(), webserv::GET);
// ASSERT_STREQ(httpRequest->getResource().c_str(), "/usr/home");
// ASSERT_STREQ(httpRequest->getVersion().c_str(), "HTTP/1.1");
// ASSERT_STREQ(httpRequest->getHeader("Host").c_str(), "x");
// ASSERT_STREQ(httpRequest->getHeader("Content-Length").c_str(), " 120");
// ASSERT_STREQ(httpRequest->getContent().c_str(),
// "........................................................................................................................");

// this->client->send_message(req2);
// smt::shared_ptr<HTTPRequest> res2 = this->connection->recv();
// ASSERT_NE(NULL, res2);
// EXPECT_EQ(webserv::POST, res2->getMethod());
// EXPECT_STREQ("/cgi-bin/process.cgi", res2->getResource().c_str());
// EXPECT_STREQ("HTTP/1.1", res2->getVersion().c_str());

// ASSERT_FALSE(res2->getHeader("Host").empty());
// EXPECT_STREQ(res2->getHeader("Host").c_str(), " www.tutorialspoint.com");
// ASSERT_FALSE(res2->getHeader("User-Agent").empty());
// EXPECT_STREQ(res2->getHeader("User-Agent").c_str(), " Mozilla/4.0
// (compatible; MSIE5.01; Windows NT)");
// ASSERT_FALSE(res2->getHeader("Content-Type").empty());
// EXPECT_STREQ(res2->getHeader("Content-Type").c_str(), " text/xml;
// charset=utf-8"); ASSERT_FALSE(res2->getHeader("Content-Length").empty());
// EXPECT_STREQ(res2->getHeader("Content-Length").c_str(), " 95");
// ASSERT_FALSE(res2->getHeader("Accept-Language").empty());
// EXPECT_STREQ(res2->getHeader("Accept-Language").c_str(), " en-us");
// ASSERT_FALSE(res2->getHeader("Accept-Encoding").empty());
// EXPECT_STREQ(res2->getHeader("Accept-Encoding").c_str(), " gzip,
// deflate"); ASSERT_FALSE(res2->getHeader("Connection").empty());
// EXPECT_STREQ(res2->getHeader("Connection").c_str(), " Keep-Alive");

// EXPECT_STREQ(res2->getContent().c_str(), "<?xml version=\"1.0\"
// encoding=\"utf-8\"?>\r\n<string
// xmlns=\"http://clearforest.com/\">string</string>");

// }

// TEST_F(test_SocketConnection, sendTwoGetTwo) {
//     ASSERT_NE(this->connection, nullptr);

// this->client->send_message("GET /usr/home
// HTTP/1.1\r\nHost:x\r\nContent-Length:
// 120\r\n\r\n........................................................................................................................");
// this->client->send_message(req2);

// auto httpRequest = this->connection->recv();
// ASSERT_NE(httpRequest, NULL);
// ASSERT_EQ(httpRequest->getMethod(), webserv::GET);
// ASSERT_STREQ(httpRequest->getResource().c_str(), "/usr/home");
// ASSERT_STREQ(httpRequest->getVersion().c_str(), "HTTP/1.1");
// ASSERT_STREQ(httpRequest->getHeader("Host").c_str(), "x");
// ASSERT_STREQ(httpRequest->getHeader("Content-Length").c_str(), " 120");
// ASSERT_STREQ(httpRequest->getContent().c_str(),
// "........................................................................................................................");

// smt::shared_ptr<HTTPRequest> res2 = this->connection->recv();
// ASSERT_NE(NULL, res2);
// EXPECT_EQ(webserv::POST, res2->getMethod());
// EXPECT_STREQ("/cgi-bin/process.cgi", res2->getResource().c_str());
// EXPECT_STREQ("HTTP/1.1", res2->getVersion().c_str());

// ASSERT_FALSE(res2->getHeader("Host").empty());
// EXPECT_STREQ(res2->getHeader("Host").c_str(), " www.tutorialspoint.com");
// ASSERT_FALSE(res2->getHeader("User-Agent").empty());
// EXPECT_STREQ(res2->getHeader("User-Agent").c_str(), " Mozilla/4.0
// (compatible; MSIE5.01; Windows NT)");
// ASSERT_FALSE(res2->getHeader("Content-Type").empty());
// EXPECT_STREQ(res2->getHeader("Content-Type").c_str(), " text/xml;
// charset=utf-8"); ASSERT_FALSE(res2->getHeader("Content-Length").empty());
// EXPECT_STREQ(res2->getHeader("Content-Length").c_str(), " 95");
// ASSERT_FALSE(res2->getHeader("Accept-Language").empty());
// EXPECT_STREQ(res2->getHeader("Accept-Language").c_str(), " en-us");
// ASSERT_FALSE(res2->getHeader("Accept-Encoding").empty());
// EXPECT_STREQ(res2->getHeader("Accept-Encoding").c_str(), " gzip,
// deflate"); ASSERT_FALSE(res2->getHeader("Connection").empty());
// EXPECT_STREQ(res2->getHeader("Connection").c_str(), " Keep-Alive");

// EXPECT_STREQ(res2->getContent().c_str(), "<?xml version=\"1.0\"
// encoding=\"utf-8\"?>\r\n<string
// xmlns=\"http://clearforest.com/\">string</string>");
// }

// Our server doesnt really handle responses right now :)
// TEST_F(test_SocketConnection, send) {
//     ASSERT_NE(this->connection, nullptr);
//     ASSERT_FALSE(this->connection->send(HTTP_RES)) << errno;
//     ASSERT_STREQ(client->receive_message().c_str(), HTTP_RES) << errno;
// }
