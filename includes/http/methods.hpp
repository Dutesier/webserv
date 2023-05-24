#pragma once

#include "config/Blocks.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "utils/smt.hpp"

namespace webserv::methods {

smt::shared_ptr<HTTPResponse> GET(smt::shared_ptr<HTTPRequest>&   request,
                                  smt::shared_ptr<LocationBlock>& location);

smt::shared_ptr<HTTPResponse> POST(smt::shared_ptr<HTTPRequest>&   request,
                                   smt::shared_ptr<LocationBlock>& location);

smt::shared_ptr<HTTPResponse> DELETE(smt::shared_ptr<HTTPRequest>&   request,
                                     smt::shared_ptr<LocationBlock>& location);

std::string generateAutoIndex(std::string directoryPath);
bool        readContentsFromFileToResponse(std::string const&             filepath,
                                           smt::shared_ptr<HTTPResponse>& response);

} // namespace webserv::methods