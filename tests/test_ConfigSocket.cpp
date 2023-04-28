#include "config/ConfigSocket.hpp"

#include <gtest/gtest.h>

class testConfigSocket : public ::testing::Test {
    public:

        void SetUp(void) {
            block1 = smt::make_shared(new webserv::ServerBlock());
            block2 = smt::make_shared(new webserv::ServerBlock());
            block3 = smt::make_shared(new webserv::ServerBlock());

            block1->m_port = "8080";
            block1->m_host = "127.0.0.1";
            block1->m_server_name = "domain1.com";
            block1->m_location.insert(std::make_pair(
                "/", smt::make_shared(new webserv::LocationBlock("/"))));

            block2->m_port = "8080";
            block2->m_host = "127.0.0.1";
            block2->m_server_name = "domain1.net";
            block2->m_location.insert(std::make_pair(
                "/", smt::make_shared(new webserv::LocationBlock("/"))));

            block3->m_port = "8081";
            block3->m_host = "127.0.0.1";

            block3->m_location["/"] =
                smt::make_shared(new webserv::LocationBlock("/"));
            block3->m_location["/cgi"] =
                smt::make_shared(new webserv::LocationBlock("/cgi"));
            block3->m_location["/cgi/python"] =
                smt::make_shared(new webserv::LocationBlock("/cgi/python"));

            m_blocks.push_back(block1);
            m_blocks.push_back(block2);
            m_blocks.push_back(block3);
        }

    protected:

        smt::shared_ptr<webserv::ServerBlock>                block1;
        smt::shared_ptr<webserv::ServerBlock>                block2;
        smt::shared_ptr<webserv::ServerBlock>                block3;
        std::vector< smt::shared_ptr<webserv::ServerBlock> > m_blocks;
};

TEST_F(testConfigSocket, getSpecs) {
    std::set< std::pair<int, std::string> > cmp;

    cmp.insert(std::make_pair(8080, "127.0.0.1"));
    cmp.insert(std::make_pair(8081, "127.0.0.1"));

    ASSERT_EQ(webserv::ConfigSocket::getSpecs().size(), 0);
    webserv::ConfigSocket::setBlocks(m_blocks);
    ASSERT_EQ(webserv::ConfigSocket::getSpecs().size(), 2);
    ASSERT_EQ(webserv::ConfigSocket::getSpecs(), cmp);
}

TEST_F(testConfigSocket, getAddress) {

    webserv::ConfigSocket::setBlocks(m_blocks);

    std::set< std::pair<int, std::string> > specs =
        webserv::ConfigSocket::getSpecs();

    std::set< std::pair<int, std::string> >::iterator it;
    for (it = specs.begin(); it != specs.end(); it++) {
        smt::shared_ptr<webserv::ServerAddress> addr =
            webserv::ConfigSocket::getAddress(*it);
        ASSERT_EQ(addr->getPort(), (*it).first);
        ASSERT_EQ(addr->getHost(), (*it).second);
    }
}

TEST_F(testConfigSocket, getConfigBlock) {

    webserv::ConfigSocket::setBlocks(m_blocks);

    std::set< std::pair<int, std::string> > specs =
        webserv::ConfigSocket::getSpecs();

    std::set< std::pair<int, std::string> >::iterator it;
    for (it = specs.begin(); it != specs.end(); it++) {
        smt::shared_ptr<webserv::ServerBlock> block =
            webserv::ConfigSocket::getConfigBlock((*it).first, (*it).second);
        ASSERT_EQ(block->m_resolvPort, (*it).first);
        ASSERT_EQ(block->m_resolvHost, (*it).second);
    }

    ASSERT_EQ(webserv::ConfigSocket::getConfigBlock(8080, "127.0.0.1"), block1);

    ASSERT_EQ(
        webserv::ConfigSocket::getConfigBlock(8080, "127.0.0.1", "domain1.com"),
        block1);

    ASSERT_EQ(
        webserv::ConfigSocket::getConfigBlock(8080, "127.0.0.1", "domain1.net"),
        block2);

    ASSERT_EQ(
        webserv::ConfigSocket::getConfigBlock(8080, "127.0.0.1", "domain1.pt"),
        block1);

    ASSERT_EQ(webserv::ConfigSocket::getConfigBlock(8080, "127.0.0.1"), block3);

    ASSERT_THROW(webserv::ConfigSocket::getConfigBlock(8083, "127.0.0.1"),
                 webserv::ConfigSocket::NoSuchBlockException);
}

TEST_F(testConfigSocket, getLocationBlock) {

    webserv::ConfigSocket::setBlocks(m_blocks);

    ASSERT_EQ(
        webserv::ConfigSocket::getLocationBlock(8081, "127.0.0.1", "/test.py"),
        block3->m_location["/"]);

    ASSERT_EQ(
        webserv::ConfigSocket::getLocationBlock(8081, "127.0.0.1", "test.py"),
        block3->m_location["/"]);

    ASSERT_EQ(webserv::ConfigSocket::getLocationBlock(8081, "127.0.0.1",
                                                      "python/test.py"),
              block3->m_location["/"]);

    ASSERT_EQ(webserv::ConfigSocket::getLocationBlock(8081, "127.0.0.1",
                                                      "cgi/python/test.py"),
              block3->m_location["/cgi/python"]);

    ASSERT_EQ(webserv::ConfigSocket::getLocationBlock(8081, "127.0.0.1",
                                                      "cgi/php/test.php"),
              block3->m_location["/cgi"]);

    ASSERT_EQ(webserv::ConfigSocket::getLocationBlock(
                  8080, "127.0.0.1", "/cgi/test.py", "domain1.com"),
              block1);

    ASSERT_EQ(webserv::ConfigSocket::getLocationBlock(
                  8080, "127.0.0.1", "/cgi/test.py", "domain1.net"),
              block2);
}