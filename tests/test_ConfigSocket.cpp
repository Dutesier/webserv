#define GTEST_TESTING 1

#include "config/Config.hpp"
#include "config/ConfigSocket.hpp"

#include <gtest/gtest.h>

class test_ConfigSocket : public testing::Test {

    public:

        void SetUp(void) {

            // making first webserv::ServerBlock
            m_srv1 =
                smt::shared_ptr<webserv::ServerBlock>(new webserv::ServerBlock);
            m_srv1->m_port = "8080";
            m_srv1->m_host = "127.0.0.1";
            m_srv3->m_server_name = "example1.com";
            m_blocks.push_back(m_srv1);

            // making second webserv::ServerBlock
            m_srv2 =
                smt::shared_ptr<webserv::ServerBlock>(new webserv::ServerBlock);
            m_srv2->m_port = "8080";
            m_srv2->m_host = "127.0.0.1";
            m_srv3->m_server_name = "example1.net";
            m_blocks.push_back(m_srv2);

            // making third webserv::ServerBlock
            m_srv3 =
                smt::shared_ptr<webserv::ServerBlock>(new webserv::ServerBlock);
            m_srv3->m_port = "8080";
            m_srv3->m_host = "127.0.0.1";
            m_srv3->m_server_name = "example1.pt";
            m_blocks.push_back(m_srv3);

            // making forth webserv::ServerBlock
            m_srv4 =
                smt::shared_ptr<webserv::ServerBlock>(new webserv::ServerBlock);
            m_srv4->m_port = "8081";
            m_srv4->m_host = "127.0.0.1";
            m_srv4->m_server_name = "example2.com";
            m_blocks.push_back(m_srv4);

            // making fifth webserv::ServerBlock
            m_srv5 =
                smt::shared_ptr<webserv::ServerBlock>(new webserv::ServerBlock);
            m_srv5->m_port = "8081";
            m_srv5->m_host = "127.0.0.1";
            m_srv5->m_server_name = "example2.net";
            m_blocks.push_back(m_srv5);

            // making sixth webserv::ServerBlock
            m_srv6 =
                smt::shared_ptr<webserv::ServerBlock>(new webserv::ServerBlock);
            m_srv6->m_port = "8081";
            m_srv6->m_host = "127.0.0.1";
            m_srv6->m_server_name = "example2.pt";
            m_blocks.push_back(m_srv6);

            // making seventh webserv::ServerBlock
            m_srv7 =
                smt::shared_ptr<webserv::ServerBlock>(new webserv::ServerBlock);
            m_srv7->m_port = "443";
            m_srv7->m_host = "127.0.0.1";
            m_blocks.push_back(m_srv7);
        }

        void TearDown(void) {}

    protected:

        std::vector< smt::shared_ptr<webserv::ServerBlock> > m_blocks;

        smt::shared_ptr<webserv::ServerBlock> m_srv1;
        smt::shared_ptr<webserv::ServerBlock> m_srv2;
        smt::shared_ptr<webserv::ServerBlock> m_srv3;
        smt::shared_ptr<webserv::ServerBlock> m_srv4;
        smt::shared_ptr<webserv::ServerBlock> m_srv5;
        smt::shared_ptr<webserv::ServerBlock> m_srv6;
        smt::shared_ptr<webserv::ServerBlock> m_srv7;
};

TEST_F(test_ConfigSocket, constructor) {

    smt::shared_ptr<webserv::ConfigSocket> config(
        new webserv::ConfigSocket(m_blocks));

    // creating stl for comparisson
    std::set< std::pair<unsigned, std::string> > comp;
    comp.insert(std::make_pair(8080, "127.0.0.1"));
    comp.insert(std::make_pair(8081, "127.0.0.1"));
    comp.insert(std::make_pair(443, "127.0.0.1"));

    ASSERT_EQ(config->m_specs, comp);
}

TEST_F(test_ConfigSocket, destructor) {

    webserv::ConfigSocket* config(new webserv::ConfigSocket(m_blocks));
    ASSERT_NO_THROW(delete config);
}

TEST_F(test_ConfigSocket, blocks) {

    smt::shared_ptr<webserv::ConfigSocket> config(
        new webserv::ConfigSocket(m_blocks));

    // structure for comparing values
    std::vector< smt::shared_ptr<webserv::ServerBlock> > comp;

    // testing 8080 "127.0.0.1"
    comp.push_back(m_srv1);
    comp.push_back(m_srv2);
    comp.push_back(m_srv3);
    comp.push_back(m_srv4);
    ASSERT_EQ(config->blocks(std::make_pair(8080, "127.0.0.1")), comp);

    // testing 8080 "127.0.0.1"
    comp.clear();
    comp.push_back(m_srv5);
    comp.push_back(m_srv6);
    ASSERT_EQ(config->blocks(std::make_pair(8081, "127.0.0.1")), comp);

    // testing 443 "127.0.0.1"
    comp.clear();
    comp.push_back(m_srv7);
    ASSERT_EQ(config->blocks(std::make_pair(443, "127.0.0.1")), comp);

    ASSERT_THROW(config->blocks(std::make_pair(555, "whatever")),
                 webserv::ConfigSocket::NoSuchSpecsException);
}
