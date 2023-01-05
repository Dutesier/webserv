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
            int fd = this->sock->accept();
            this->connection = this->sock->connection(fd);
        }

        void TearDown(void) {
            delete this->sock;
            delete this->client;
        }

    protected:

        Client*                    client;
        webserv::TCPSocket*        sock;
        webserv::SocketConnection* connection;

        std::string req1 = "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n";
        std::string req2 = "POST /cgi-bin/process.cgi HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nContent-Type: text/xml; charset=utf-8\r\nContent-Length: 95\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<string xmlns=\"http://clearforest.com/\">string</string>";
        std::string req3 = "GET /hello.htm HTTP/1.0\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\r\n\r\n";
        std::string reqNoMethod = "/hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\r\n\r\n";
        std::string reqNoPath = "GET HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\r\n\r\n";
        std::string reqNoVersion = "GET /hello.htm \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\r\n\r\n";
        std::string reqCrazyFirstLine = "ajsadasdhaa a23\r\nGET /hello.htm \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\r\n\r\n";
};

TEST_F(test_SocketConnection, constructor) {
    ASSERT_NE(this->connection, nullptr);
}

TEST_F(test_SocketConnection, destructor) {
    ASSERT_NE(this->connection, nullptr);
}

TEST_F(test_SocketConnection, close) {
    ASSERT_NE(this->connection, nullptr);
    ASSERT_NO_THROW(this->connection->close()) << errno;
    ASSERT_EQ(this->connection->sockfd(), -1);
    std::string message = "HTTP/1.1 404\r\nContent-Length: 0\r\n";
    ASSERT_THROW(this->connection->send(message),
                 webserv::SocketConnection::SendFailureException)
        << errno;
}

TEST_F(test_SocketConnection, recv) {
    ASSERT_NE(this->connection, nullptr);
    this->client->send_message(HTTP_REQ);

    auto httpRequest = this->connection->recv();
    ASSERT_NE(httpRequest, NULL);
    ASSERT_EQ(httpRequest->method, 1);
    ASSERT_EQ(httpRequest->resource, "/");
    ASSERT_EQ(httpRequest->version, "HTTP/1.1");
    ASSERT_EQ(httpRequest->headers.find("Host")->second, "x");
}

TEST_F(test_SocketConnection, hardcore) {
    ASSERT_NE(this->connection, nullptr);
    this->client->send_message("GET /usr/home HTTP/1.1\r\nHost:x\r\nContent-Length: 120\r\n\r\n........................................................................................................................"); 

    auto httpRequest = this->connection->recv();
    ASSERT_NE(httpRequest, NULL);
    ASSERT_EQ(httpRequest->method, 1);
    ASSERT_EQ(httpRequest->resource, "/usr/home");
    ASSERT_EQ(httpRequest->version, "HTTP/1.1");
    ASSERT_EQ(httpRequest->headers.find("Host")->second, "x");
    ASSERT_EQ(httpRequest->headers.find("Content-Length")->second, " 120");
    ASSERT_EQ(httpRequest->content, "........................................................................................................................");

    this->client->send_message(req2);
    smt::shared_ptr<HTTPRequest> res2 = this->connection->recv();
    ASSERT_NE(NULL, res2);
    EXPECT_EQ(2, res2->method);
    EXPECT_STREQ("/cgi-bin/process.cgi", res2->resource.c_str());
    EXPECT_STREQ("HTTP/1.1", res2->version.c_str());

    ASSERT_TRUE(res2->headers.find("Host") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Host")->second.c_str(), " www.tutorialspoint.com");
    ASSERT_TRUE(res2->headers.find("User-Agent") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("User-Agent")->second.c_str(), " Mozilla/4.0 (compatible; MSIE5.01; Windows NT)");
    ASSERT_TRUE(res2->headers.find("Content-Type") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Content-Type")->second.c_str(), " text/xml; charset=utf-8");
    ASSERT_TRUE(res2->headers.find("Content-Length") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Content-Length")->second.c_str(), " 95");
    ASSERT_TRUE(res2->headers.find("Accept-Language") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Accept-Language")->second.c_str(), " en-us");
    ASSERT_TRUE(res2->headers.find("Accept-Encoding") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Accept-Encoding")->second.c_str(), " gzip, deflate");
    ASSERT_TRUE(res2->headers.find("Connection") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Connection")->second.c_str(), " Keep-Alive");

    EXPECT_STREQ(res2->content.c_str(), "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<string xmlns=\"http://clearforest.com/\">string</string>");

}

TEST_F(test_SocketConnection, sendTwoGetTwo) {
    ASSERT_NE(this->connection, nullptr);

    this->client->send_message("GET /usr/home HTTP/1.1\r\nHost:x\r\nContent-Length: 120\r\n\r\n........................................................................................................................"); 
    this->client->send_message(req2);

    auto httpRequest = this->connection->recv();
    ASSERT_NE(httpRequest, NULL);
    ASSERT_EQ(httpRequest->method, 1);
    ASSERT_EQ(httpRequest->resource, "/usr/home");
    ASSERT_EQ(httpRequest->version, "HTTP/1.1");
    ASSERT_EQ(httpRequest->headers.find("Host")->second, "x");
    ASSERT_EQ(httpRequest->headers.find("Content-Length")->second, " 120");
    ASSERT_EQ(httpRequest->content, "........................................................................................................................");

    smt::shared_ptr<HTTPRequest> res2 = this->connection->recv();
    ASSERT_NE(NULL, res2);
    EXPECT_EQ(2, res2->method);
    EXPECT_STREQ("/cgi-bin/process.cgi", res2->resource.c_str());
    EXPECT_STREQ("HTTP/1.1", res2->version.c_str());

    ASSERT_TRUE(res2->headers.find("Host") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Host")->second.c_str(), " www.tutorialspoint.com");
    ASSERT_TRUE(res2->headers.find("User-Agent") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("User-Agent")->second.c_str(), " Mozilla/4.0 (compatible; MSIE5.01; Windows NT)");
    ASSERT_TRUE(res2->headers.find("Content-Type") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Content-Type")->second.c_str(), " text/xml; charset=utf-8");
    ASSERT_TRUE(res2->headers.find("Content-Length") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Content-Length")->second.c_str(), " 95");
    ASSERT_TRUE(res2->headers.find("Accept-Language") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Accept-Language")->second.c_str(), " en-us");
    ASSERT_TRUE(res2->headers.find("Accept-Encoding") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Accept-Encoding")->second.c_str(), " gzip, deflate");
    ASSERT_TRUE(res2->headers.find("Connection") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Connection")->second.c_str(), " Keep-Alive");

    EXPECT_STREQ(res2->content.c_str(), "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<string xmlns=\"http://clearforest.com/\">string</string>");
}

// Our server doesnt really handle responses right now :)
// TEST_F(test_SocketConnection, send) {
//     ASSERT_NE(this->connection, nullptr);
//     ASSERT_TRUE(this->connection->send(HTTP_RES)) << errno;
//     ASSERT_STREQ(client->receive_message().c_str(), HTTP_RES) << errno;
// }
