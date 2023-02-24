#include <cgi/CGIContext.hpp>
#include <http/HTTPRequest.hpp>
#include <gtest/gtest.h>

TEST(CGIContext, DoesntLeak) {
    HTTPRequest req;

    req.setResource("/home?key=value");
    cgi::CGIContext ctx(req.getQueriesFromResource(), req.getRefinedResource());
}