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

    smt::shared_ptr<webserv::ServerBlock> cmp =
        webserv::ConfigSocket::getConfigBlock(8080, "127.0.0.1");
    ASSERT_TRUE(cmp->m_host == block1->m_host &&
                cmp->m_port == block1->m_port &&
                cmp->m_server_name == block1->m_server_name);

    cmp =
        webserv::ConfigSocket::getConfigBlock(8080, "127.0.0.1", "domain1.com");
    ASSERT_TRUE(cmp->m_host == block1->m_host &&
                cmp->m_port == block1->m_port &&
                cmp->m_server_name == block1->m_server_name);

    cmp =
        webserv::ConfigSocket::getConfigBlock(8080, "127.0.0.1", "domain1.net");
    ASSERT_TRUE(cmp->m_host == block2->m_host &&
                cmp->m_port == block2->m_port &&
                cmp->m_server_name == block2->m_server_name);

    cmp =
        webserv::ConfigSocket::getConfigBlock(8080, "127.0.0.1", "domain1.pt");
    ASSERT_TRUE(cmp->m_host == block1->m_host &&
                cmp->m_port == block1->m_port &&
                cmp->m_server_name == block1->m_server_name);

    cmp = webserv::ConfigSocket::getConfigBlock(8081, "127.0.0.1");
    ASSERT_TRUE(cmp->m_host == block3->m_host &&
                cmp->m_port == block3->m_port &&
                cmp->m_server_name == block3->m_server_name);

    ASSERT_THROW(webserv::ConfigSocket::getConfigBlock(8083, "127.0.0.1"),
                 webserv::ConfigSocket::NoSuchBlockException);
}

TEST_F(testConfigSocket, getLocationBlock) {

    webserv::ConfigSocket::setBlocks(m_blocks);

    smt::shared_ptr<webserv::LocationBlock> cmp =
        webserv::ConfigSocket::getLocationBlock(block1, "/test.py");
    ASSERT_TRUE(cmp->m_target == block1->m_location["/"]->m_target);

    cmp = webserv::ConfigSocket::getLocationBlock(block3, "/test.py");
    ASSERT_TRUE(cmp->m_target == block3->m_location["/"]->m_target);

    cmp = webserv::ConfigSocket::getLocationBlock(block3, "python/test.py");
    ASSERT_TRUE(cmp->m_target == block3->m_location["/"]->m_target);

    cmp = webserv::ConfigSocket::getLocationBlock(block3, "cgi/python/test.py");
    ASSERT_TRUE(cmp->m_target == block3->m_location["/cgi/python"]->m_target);

    cmp = webserv::ConfigSocket::getLocationBlock(block3, "cgi/php/test.php");
    ASSERT_TRUE(cmp->m_target == block3->m_location["/cgi"]->m_target);
}

// TEST_F(testConfigSocket, getLocationBlock) {

// webserv::ConfigSocket::setBlocks(m_blocks);

// smt::shared_ptr<webserv::LocationBlock> cmp =
//     webserv::ConfigSocket::getLocationBlock(block3, "cgi/python/test.py");
// ASSERT_TRUE(cmp->m_target == block1->m_location["/cgi/python"]->m_target);
// }