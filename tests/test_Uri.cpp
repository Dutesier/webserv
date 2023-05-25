#include "http/Uri.hpp"
#include "utils/smt.hpp"

#include <gtest/gtest.h>

TEST(test_uri, constructor) {

    smt::shared_ptr<Uri> uri;

    // testing uri in absolute-form without query
    uri = smt::shared_ptr<Uri>(
        new Uri("http://www.example.org/pub/WWW/TheProject.html"));
    EXPECT_EQ(uri->m_scheme, "http") << uri->m_scheme;
    EXPECT_EQ(uri->m_host, "www.example.org") << uri->m_host;
    EXPECT_TRUE(uri->m_port.empty()) << uri->m_port;
    EXPECT_EQ(uri->m_path, "pub/WWW/TheProject.html") << uri->m_path;
    EXPECT_TRUE(uri->m_query.empty()) << uri->m_query;

    // testing uri in absolute-form with query
    uri = smt::shared_ptr<Uri>(new Uri(
        "http://www.example.org/pub/WWW/TheProject.html?tag=networking"));
    EXPECT_EQ(uri->m_scheme, "http") << uri->m_scheme;
    EXPECT_EQ(uri->m_host, "www.example.org") << uri->m_host;
    EXPECT_TRUE(uri->m_port.empty()) << uri->m_port;
    EXPECT_EQ(uri->m_path, "pub/WWW/TheProject.html") << uri->m_path;
    EXPECT_EQ(uri->m_query, "tag=networking") << uri->m_query;

    // testing uri in origin-form without query
    uri = smt::shared_ptr<Uri>(new Uri("/where"));
    EXPECT_TRUE(uri->m_scheme.empty()) << uri->m_scheme;
    EXPECT_TRUE(uri->m_port.empty()) << uri->m_port;
    EXPECT_TRUE(uri->m_host.empty()) << uri->m_host;
    EXPECT_EQ(uri->m_path, "where") << uri->m_path;
    EXPECT_TRUE(uri->m_query.empty()) << uri->m_query;

    // testing uri in origin-form with query
    uri = smt::shared_ptr<Uri>(new Uri("/where?q=now"));
    EXPECT_TRUE(uri->m_scheme.empty()) << uri->m_scheme;
    EXPECT_TRUE(uri->m_port.empty()) << uri->m_port;
    EXPECT_TRUE(uri->m_host.empty()) << uri->m_host;
    EXPECT_EQ(uri->m_path, "where") << uri->m_path;
    EXPECT_EQ(uri->m_query, "q=now") << uri->m_query;

    // testing uri in absolute-form without query
    uri = smt::shared_ptr<Uri>(new Uri("localhost:3000/cgi/python/me.py"));
    EXPECT_TRUE(uri->m_scheme.empty()) << uri->m_scheme;
    EXPECT_EQ(uri->m_host, "localhost") << uri->m_host;
    EXPECT_EQ(uri->m_port, "3000") << uri->m_port;
    EXPECT_EQ(uri->m_path, "cgi/python/me.py") << uri->m_path;
    EXPECT_TRUE(uri->m_query.empty()) << uri->m_query;

    // testing uri in absolute-form with query
    uri = smt::shared_ptr<Uri>(
        new Uri("localhost:3000/cgi/python/me.py?tag=networking&tag=web"));
    EXPECT_TRUE(uri->m_scheme.empty()) << uri->m_scheme;
    EXPECT_EQ(uri->m_host, "localhost") << uri->m_host;
    EXPECT_EQ(uri->m_port, "3000") << uri->m_port;
    EXPECT_EQ(uri->m_path, "cgi/python/me.py") << uri->m_path;
    EXPECT_EQ(uri->m_query, "tag=networking&tag=web") << uri->m_query;
}

TEST(test_uri, testRoute) {
    smt::shared_ptr<Uri> uri =
        smt::shared_ptr<Uri>(new Uri("localhost:3000/cgi/python/me.py"));
    uri->route("cgi/python/", "webserv/cgi/python");
    EXPECT_EQ(uri->m_routedPath, "webserv/cgi/python/me.py")
        << uri->m_routedPath;

    uri = smt::shared_ptr<Uri>(new Uri("localhost:3000/cgi/python/me.py"));
    uri->route("cgi/python", "webserv/cgi/python");
    EXPECT_EQ(uri->m_routedPath, "webserv/cgi/python/me.py")
        << uri->m_routedPath;

    uri = smt::shared_ptr<Uri>(new Uri("localhost:3000/cgi/me.py"));
    uri->route("cgi", "webserv/cgi/python");
    EXPECT_EQ(uri->m_routedPath, "webserv/cgi/python/me.py")
        << uri->m_routedPath;

    uri = smt::shared_ptr<Uri>(new Uri("localhost:3000/cgi/me.py"));
    uri->route("webserv", "webserv/cgi/python");
    EXPECT_EQ(uri->m_routedPath, uri->m_path) << uri->m_routedPath;

    uri = smt::shared_ptr<Uri>(new Uri("localhost:3000/"));
    uri->route("/", "webserv/cgi/python");
    EXPECT_EQ(uri->m_routedPath, "webserv/cgi/python") << uri->m_routedPath;

    uri = smt::shared_ptr<Uri>(new Uri("/"));
    uri->route("/", "webserv/cgi/python");
    EXPECT_EQ(uri->m_routedPath, "webserv/cgi/python") << uri->m_routedPath;

    uri = smt::shared_ptr<Uri>(new Uri("/"));
    uri->route("/", "webserv/cgi/python");
    EXPECT_EQ(uri->m_routedPath, "webserv/cgi/python") << uri->m_routedPath;
}