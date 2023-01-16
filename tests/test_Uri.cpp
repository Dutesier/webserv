#include "http/Uri.hpp"
#include "utils/smt.hpp"

#include <gtest/gtest.h>

TEST(testUri, Constructor) {

	smt::shared_ptr<webserv::Uri> uri;

	// testing URI in absolute-form without query
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri("http://www.example.org/pub/WWW/TheProject.html"));
	ASSERT_EQ(uri->m_scheme, "http") << uri->m_scheme;
	ASSERT_EQ(uri->m_host, "www.example.org") << uri->m_host;
	ASSERT_EQ(uri->m_path, "/pub/WWW/TheProject.html") << uri->m_path;
	ASSERT_TRUE(uri->m_query.empty()) << uri->m_query;

	// testing URI in absolute-form with query
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri("http://www.example.org/pub/WWW/TheProject.html?tag=networking"));
	ASSERT_EQ(uri->m_scheme, "http") << uri->m_scheme;
	ASSERT_EQ(uri->m_host, "www.example.org") << uri->m_host;
	ASSERT_EQ(uri->m_path, "/pub/WWW/TheProject.html") << uri->m_path;
	ASSERT_EQ(uri->m_query, "tag=networking") << uri->m_query;

	// testing URI in origin-form without query
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri("/where"));
	ASSERT_TRUE(uri->m_scheme.empty()) << uri->m_scheme;
	ASSERT_TRUE(uri->m_host.empty()) << uri->m_host;
	ASSERT_EQ(uri->m_path, "/where") << uri->m_path;
	ASSERT_TRUE(uri->m_query.empty()) << uri->m_query;

	// testing URI in origin-form with query
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri("/where?tag=networking"));
	ASSERT_TRUE(uri->m_scheme.empty()) << uri->m_scheme;
	ASSERT_TRUE(uri->m_host.empty()) << uri->m_host;
	ASSERT_EQ(uri->m_path, "/where") << uri->m_path;
	ASSERT_EQ(uri->m_query, "tag=networking") << uri->m_query;
}

TEST(testUri, to_str) {

	smt::shared_ptr<webserv::Uri> uri;

	std::string str = "http://www.example.org/pub/WWW/TheProject.html";
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri(str));
	ASSERT_EQ(uri->to_str(), str);

	str = "http://www.example.org/pub/WWW/TheProject.html?tag=networking";
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri(str));
	ASSERT_EQ(uri->to_str(), str);

	str = "/where";
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri(str));
	ASSERT_EQ(uri->to_str(), str);

	str = "/where?tag=networking";
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri(str));
	ASSERT_EQ(uri->to_str(), str);
}

TEST(testUri, path_w_root) {

	smt::shared_ptr<webserv::Uri> uri;

	std::string str = "http://www.example.org/pub/WWW/TheProject.html";
	std::string root = "../webserv/website/";
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri(str));
	ASSERT_EQ(uri->path_w_root(root), "../webserv/website/pub/WWW/TheProject.html");

	str = "http://www.example.org/pub/WWW/TheProject.html?tag=networking";
	root = "../webserv/website";
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri(str));
	ASSERT_EQ(uri->path_w_root(root), "../webserv/website/pub/WWW/TheProject.html");

	str = "/where";
	root = "../webserv/website/";
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri(str));
	ASSERT_EQ(uri->path_w_root(root), "../webserv/website/where");

	str = "/where?tag=networking";
	root = "../webserv/website";
	uri = smt::shared_ptr<webserv::Uri>(new webserv::Uri(str));
	ASSERT_EQ(uri->path_w_root(root), "../webserv/website/where");
}
