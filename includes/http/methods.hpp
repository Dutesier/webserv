#pragma once

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "config/Blocks.hpp"
#include "utils/smt.hpp"

namespace webserv::methods
{

smt::shared_ptr<HTTPResponse>
GET(smt::shared_ptr<HTTPRequest>& request,
                    smt::shared_ptr<LocationBlock>& location);

// smt::shared_ptr<HTTPResponse>
// POST(smt::shared_ptr<HTTPRequest>& request,
//                     smt::shared_ptr<LocationBlock>& location);

// smt::shared_ptr<HTTPResponse>
// HEAD(smt::shared_ptr<HTTPRequest>& request,
//                     smt::shared_ptr<LocationBlock>& location);

// smt::shared_ptr<HTTPResponse>
// DELETE(smt::shared_ptr<HTTPRequest>& request,
//                     smt::shared_ptr<LocationBlock>& location);

}