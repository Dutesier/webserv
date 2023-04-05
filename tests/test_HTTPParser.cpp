// #include "http/HTTPParser.hpp"

// #include <gtest/gtest.h>
// #include <iostream>

// class test_HTTPParser : public testing::Test {
//     public:

//         void SetUp(void) { this->parser = new HTTPParser(); }

//         void TearDown(void) { delete this->parser; }

//     protected:

//         HTTPParser* parser;
//         std::string req1 = "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 "
//                            "(compatible; MSIE5.01; Windows NT)\r\nHost: "
//                            "www.tutorialspoint.com\r\nAccept-Language: "
//                            "en-us\r\nAccept-Encoding: gzip, "
//                            "deflate\r\nConnection: Keep-Alive\r\n\r\n";
//         std::string req2 =
//             "POST /cgi-bin/process.cgi HTTP/1.1\r\nUser-Agent: Mozilla/4.0 "
//             "(compatible; MSIE5.01; Windows NT)\r\nHost: "
//             "www.tutorialspoint.com\r\nContent-Type: text/xml; "
//             "charset=utf-8\r\nContent-Length: 96\r\nAccept-Language: "
//             "en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: "
//             "Keep-Alive\r\n\r\n<?xml version=\"1.0\" "
//             "encoding=\"utf-8\"?>\r\n<string "
//             "xmlns=\"http://clearforest.com/\">string</string>";
//         std::string req3 = "GET /hello.htm HTTP/1.0\r\nUser-Agent: Mozilla/4.0 "
//                            "(compatible; MSIE5.01; Windows "
//                            "NT)\r\nAccept-Language: en-us\r\nAccept-Encoding: "
//                            "gzip, deflate\nConnection: Keep-Alive\r\n\r\n";
//         std::string reqNoMethod =
//             "/hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; "
//             "MSIE5.01; Windows NT)\r\nHost: "
//             "www.tutorialspoint.com\r\nAccept-Language: "
//             "en-us\r\nAccept-Encoding: gzip, deflate\nConnection: "
//             "Keep-Alive\r\n\r\n";
//         std::string reqNoPath =
//             "GET HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; "
//             "Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: "
//             "en-us\r\nAccept-Encoding: gzip, deflate\nConnection: "
//             "Keep-Alive\r\n\r\n";
//         std::string reqNoVersion =
//             "GET /hello.htm \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; "
//             "Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: "
//             "en-us\r\nAccept-Encoding: gzip, deflate\nConnection: "
//             "Keep-Alive\r\n\r\n";
//         std::string reqCrazyFirstLine =
//             "ajsadasdhaa a23\r\nGET /hello.htm \r\nUser-Agent: Mozilla/4.0 "
//             "(compatible; MSIE5.01; Windows NT)\r\nHost: "
//             "www.tutorialspoint.com\r\nAccept-Language: "
//             "en-us\r\nAccept-Encoding: gzip, deflate\nConnection: "
//             "Keep-Alive\r\n\r\n";
// };

// TEST_F(test_HTTPParser, getMethod) {

//     smt::shared_ptr<HTTPRequest> res1 = parser->getNextRequest(req1);
//     ASSERT_TRUE(res1->isValid());
//     EXPECT_EQ(1, res1->getMethod());

//     smt::shared_ptr<HTTPRequest> res2 = parser->getNextRequest(req2);
//     ASSERT_TRUE(res2->isValid());
//     EXPECT_EQ(2, res2->getMethod());

//     smt::shared_ptr<HTTPRequest> res3 = parser->getNextRequest(reqNoMethod);
//     ASSERT_FALSE(res3->isValid());
// }

// TEST_F(test_HTTPParser, getResource) {

//     smt::shared_ptr<HTTPRequest> res1 = parser->getNextRequest(req1);
//     ASSERT_TRUE(res1->isValid());
//     EXPECT_STREQ("/hello.htm", res1->getResource().c_str());

//     smt::shared_ptr<HTTPRequest> res2 = parser->getNextRequest(req2);
//     ASSERT_TRUE(res2->isValid());
//     EXPECT_STREQ("/cgi-bin/process.cgi", res2->getResource().c_str());

//     smt::shared_ptr<HTTPRequest> res3 = parser->getNextRequest(reqNoPath);
//     ASSERT_FALSE(res3->isValid());
// }

// TEST_F(test_HTTPParser, getVersion) {

//     smt::shared_ptr<HTTPRequest> res1 = parser->getNextRequest(req1);
//     ASSERT_TRUE(res1->isValid());
//     EXPECT_STREQ("HTTP/1.1", res1->getVersion().c_str());

//     smt::shared_ptr<HTTPRequest> res2 = parser->getNextRequest(req2);
//     ASSERT_TRUE(res2->isValid());
//     EXPECT_STREQ("HTTP/1.1", res2->getVersion().c_str());

//     smt::shared_ptr<HTTPRequest> res3 = parser->getNextRequest(req3);
//     ASSERT_TRUE(res3->isValid());
//     EXPECT_STREQ("HTTP/1.0", res3->getVersion().c_str());

//     smt::shared_ptr<HTTPRequest> res4 = parser->getNextRequest(reqNoVersion);
//     ASSERT_FALSE(res4->isValid());
// }

// TEST_F(test_HTTPParser, parse_header) {

//     smt::shared_ptr<HTTPRequest> res1 = parser->getNextRequest(req1);
//     ASSERT_TRUE(res1->isValid());
//     EXPECT_EQ(1, res1->getMethod());
//     EXPECT_STREQ("/hello.htm", res1->getResource().c_str());
//     EXPECT_STREQ("HTTP/1.1", res1->getVersion().c_str());

//     ASSERT_FALSE(res1->getHeader("Host").empty());
//     EXPECT_STREQ(res1->getHeader("Host").c_str(), " www.tutorialspoint.com");
//     ASSERT_FALSE(res1->getHeader("User-Agent").empty());
//     EXPECT_STREQ(res1->getHeader("User-Agent").c_str(),
//                  " Mozilla/4.0 (compatible; MSIE5.01; Windows NT)");
//     ASSERT_FALSE(res1->getHeader("Accept-Language").empty());
//     EXPECT_STREQ(res1->getHeader("Accept-Language").c_str(), " en-us");
//     ASSERT_FALSE(res1->getHeader("Accept-Encoding").empty());
//     EXPECT_STREQ(res1->getHeader("Accept-Encoding").c_str(), " gzip, deflate");
//     ASSERT_FALSE(res1->getHeader("Connection").empty());
//     EXPECT_STREQ(res1->getHeader("Connection").c_str(), " Keep-Alive");

//     smt::shared_ptr<HTTPRequest> res2 = parser->getNextRequest(req2);
//     ASSERT_TRUE(res2->isValid());
//     EXPECT_EQ(2, res2->getMethod());
//     EXPECT_STREQ("/cgi-bin/process.cgi", res2->getResource().c_str());
//     EXPECT_STREQ("HTTP/1.1", res2->getVersion().c_str());

//     ASSERT_FALSE(res2->getHeader("Host").empty());
//     EXPECT_STREQ(res2->getHeader("Host").c_str(), " www.tutorialspoint.com");
//     ASSERT_FALSE(res2->getHeader("User-Agent").empty());
//     EXPECT_STREQ(res2->getHeader("User-Agent").c_str(),
//                  " Mozilla/4.0 (compatible; MSIE5.01; Windows NT)");
//     ASSERT_FALSE(res2->getHeader("Content-Type").empty());
//     EXPECT_STREQ(res2->getHeader("Content-Type").c_str(),
//                  " text/xml; charset=utf-8");
//     ASSERT_FALSE(res2->getHeader("Content-Length").empty());
//     EXPECT_STREQ(res2->getHeader("Content-Length").c_str(), " 96");
//     ASSERT_FALSE(res2->getHeader("Accept-Language").empty());
//     EXPECT_STREQ(res2->getHeader("Accept-Language").c_str(), " en-us");
//     ASSERT_FALSE(res2->getHeader("Accept-Encoding").empty());
//     EXPECT_STREQ(res2->getHeader("Accept-Encoding").c_str(), " gzip, deflate");
//     ASSERT_FALSE(res2->getHeader("Connection").empty());
//     EXPECT_STREQ(res2->getHeader("Connection").c_str(), " Keep-Alive");
// }

// // TEST_F(test_SocketConnection, hardcore) {
// //
// //     this->client->send_message(
// //         "GET "
// //         "/usr/homeHTTP/"
// //         "1.1\r\nHost:x\r\nContent-Length:120\r\n\r\n..........................."
// //         "......................................................................"
// //         ".......................");
// //     auto httpRequest = this->connection->recv();
// //     ASSERT_NE(httpRequest, NULL);
// //     ASSERT_EQ(httpRequest->getMethod(), webserv::GET);
// //     ASSERT_STREQ(httpRequest->getResource().c_str(), "/usr/home");
// //     ASSERT_STREQ(httpRequest->getVersion().c_str(), "HTTP/1.1");
// //     ASSERT_STREQ(httpRequest->getHeader("Host").c_str(), "x");
// //     ASSERT_STREQ(httpRequest->getHeader("Content-Length").c_str(), " 120");
// //     ASSERT_STREQ(httpRequest->getContent().c_str(),
// //                  "............................................................."
// //                  "...........................................................");
// //
// //     this->client->send_message(req2);
// //     smt::shared_ptr<HTTPRequest> res2 = this->connection->recv();
// //     ASSERT_TRUE(res1->isValid());
// //     EXPECT_EQ(webserv::POST, res2->getMethod());
// //     EXPECT_STREQ("/cgi-bin/process.cgi", res2->getResource().c_str());
// //     EXPECT_STREQ("HTTP/1.1", res2->getVersion().c_str());
// //
// //     ASSERT_FALSE(res2->getHeader("Host").empty());
// //     EXPECT_STREQ(res2->getHeader("Host").c_str(), " www.tutorialspoint.com");
// //     ASSERT_FALSE(res2->getHeader("User-Agent").empty());
// //     EXPECT_STREQ(res2->getHeader("User-Agent").c_str(),
// //                  " Mozilla/4.0(compatible; MSIE5.01; Windows NT)");
// //     ASSERT_FALSE(res2->getHeader("Content-Type").empty());
// //     EXPECT_STREQ(res2->getHeader("Content-Type").c_str(),
// //                  " text/xml;charset=utf-8");
// //     ASSERT_FALSE(res2->getHeader("Content-Length").empty());
// //     EXPECT_STREQ(res2->getHeader("Content-Length").c_str(), " 95");
// //     ASSERT_FALSE(res2->getHeader("Accept-Language").empty());
// //     EXPECT_STREQ(res2->getHeader("Accept-Language").c_str(), " en-us");
// //     ASSERT_FALSE(res2->getHeader("Accept-Encoding").empty());
// //     EXPECT_STREQ(res2->getHeader("Accept-Encoding").c_str(), " gzip,
// //     deflate"); ASSERT_FALSE(res2->getHeader("Connection").empty());
// //     EXPECT_STREQ(res2->getHeader("Connection").c_str(), " Keep-Alive");
// //
// //     EXPECT_STREQ(res2->getContent().c_str(),
// //                  "<?xml "
// //                  "version=\"1.0\"encoding=\"utf-8\"?>\r\n<stringxmlns=\"http://"
// //                  "clearforest.com/\">string</string>");
// // }
// //
// // TEST_F(test_SocketConnection, sendTwoGetTwo) {
// //
// //     ASSERT_NE(this->connection, nullptr);
// //
// //     this->client->send_message(
// //         "GET "
// //         "/usr/homeHTTP/"
// //         "1.1\r\nHost:x\r\nContent-Length:120\r\n\r\n..........................."
// //         "......................................................................"
// //         ".......................");
// //     this->client->send_message(req2);
// //
// //     auto httpRequest = this->connection->recv();
// //     ASSERT_NE(httpRequest, NULL);
// //     ASSERT_EQ(httpRequest->getMethod(), webserv::GET);
// //     ASSERT_STREQ(httpRequest->getResource().c_str(), "/usr/home");
// //     ASSERT_STREQ(httpRequest->getVersion().c_str(), "HTTP/1.1");
// //     ASSERT_STREQ(httpRequest->getHeader("Host").c_str(), "x");
// //     ASSERT_STREQ(httpRequest->getHeader("Content-Length").c_str(), " 120");
// //     ASSERT_STREQ(httpRequest->getContent().c_str(),
// //                  "............................................................."
// //                  "...........................................................");
// //
// //     smt::shared_ptr<HTTPRequest> res2 = this->connection->recv();
// //     ASSERT_TRUE(res1->isValid());
// //     EXPECT_EQ(webserv::POST, res2->getMethod());
// //     EXPECT_STREQ("/cgi-bin/process.cgi", res2->getResource().c_str());
// //     EXPECT_STREQ("HTTP/1.1", res2->getVersion().c_str());
// //
// //     ASSERT_FALSE(res2->getHeader("Host").empty());
// //     EXPECT_STREQ(res2->getHeader("Host").c_str(), " www.tutorialspoint.com");
// //     ASSERT_FALSE(res2->getHeader("User-Agent").empty());
// //     EXPECT_STREQ(res2->getHeader("User-Agent").c_str(),
// //                  " Mozilla/4.0 (compatible; MSIE5.01; Windows NT)");
// //     ASSERT_FALSE(res2->getHeader("Content-Type").empty());
// //     EXPECT_STREQ(res2->getHeader("Content-Type").c_str(),
// //                  " text/xml;charset=utf-8");
// //     ASSERT_FALSE(res2->getHeader("Content-Length").empty());
// //     EXPECT_STREQ(res2->getHeader("Content-Length").c_str(), " 95");
// //     ASSERT_FALSE(res2->getHeader("Accept-Language").empty());
// //     EXPECT_STREQ(res2->getHeader("Accept-Language").c_str(), " en-us");
// //     ASSERT_FALSE(res2->getHeader("Accept-Encoding").empty());
// //     EXPECT_STREQ(res2->getHeader("Accept-Encoding").c_str(), " gzip,
// //     deflate"); ASSERT_FALSE(res2->getHeader("Connection").empty());
// //     EXPECT_STREQ(res2->getHeader("Connection").c_str(), " Keep-Alive");
// //
// //     EXPECT_STREQ(res2->getContent().c_str(),
// //                  "<?xml "
// //                  "version=\"1.0\"encoding=\"utf-8\"?>\r\n<stringxmlns=\"http://"
// //                  "clearforest.com/\">string</string>");
// // }
