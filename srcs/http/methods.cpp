#include "http/methods.hpp"
#include "http/HTTPHandler.hpp"

#include "http/pathValidation.hpp"

#include <sstream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace webserv::methods
{

smt::shared_ptr<HTTPResponse>
GET(smt::shared_ptr<HTTPRequest>& request,
                    smt::shared_ptr<LocationBlock>& location) {
    LOG_D("GET method called");
    std::string                        body;
    std::map<std::string, std::string> headers;
    smt::shared_ptr<webserv::HTTPResponse> resp;
    resp = smt::make_shared<webserv::HTTPResponse>(
        new webserv::HTTPResponse(200, headers, body));

    // Check if method is allowed
    bool allowed = location->m_allowed_methods.find("GET") != location->m_allowed_methods.end();
    if (!allowed) {
        resp->m_status = 403; // Need to check this
        resp->m_reason = resp->s_status_map[resp->m_status];
        LOG_D("Returning error");
        return resp;
    }

    // Check if file in path and access is allowed (This logic is common to CGI)
    std::string refined = request->getRefinedResource();
    std::string directory = location->m_root; // FIXME: Shouldn't we have different roots for different methods

    LOG_D("Checking requested resource: " + path::formattedFullPath(directory, refined));
    // If so, read file into buffer.
    if (webserv::path::fileExists(directory, refined) && !webserv::path::isCrawler(refined)) {

        std::string filepath = path::formattedFullPath(directory, refined);
        std::ifstream file(filepath.c_str());
        std::string line;
        std::stringstream ss;
        std::stringstream responseBody;
        if (file.good()) {
            responseBody << file.rdbuf();
            resp->m_body = responseBody.str();
            ss << (resp->m_body.length());
            resp->m_header["Content-Length"] = ss.str();
            LOG_D("Setting Content-Length: " + ss.str() + "=> " + resp->m_header["Content-Length"]);
            file.close();
            return resp;
        } else {
            resp->m_status = 500;
            resp->m_reason = resp->s_status_map[resp->m_status];
            return resp;
        }
    } else {
        resp->m_status = 403; // Need to check this
        resp->m_reason = resp->s_status_map[resp->m_status];
        return resp;
    }

    // Write buffer to HTTPResponse
    // Return HTTPResponse
}

smt::shared_ptr<HTTPResponse>
POST(smt::shared_ptr<HTTPRequest>& request,
                    smt::shared_ptr<LocationBlock>& location) {
    LOG_D("POST method called");
    std::string                        body;
    std::map<std::string, std::string> headers;
    smt::shared_ptr<webserv::HTTPResponse> resp;
    resp = smt::make_shared<webserv::HTTPResponse>(
        new webserv::HTTPResponse(200, headers, body));

    // Check if method is allowed
    bool allowed = location->m_allowed_methods.find("POST") != location->m_allowed_methods.end();
    if (!allowed) {
        resp->m_status = 403; // Need to check this
        resp->m_reason = resp->s_status_map[resp->m_status];
        LOG_D("Returning error");
        return resp;
    }

    // Check if file in path and access is allowed (This logic is common to CGI)
    std::string refined = request->getRefinedResource();
    std::string directory = location->m_root; // FIXME: Shouldn't we have different roots for different methods

    LOG_D("Checking requested resource: " + path::formattedFullPath(directory, refined));
    if (!webserv::path::isCrawler(refined)) {
        std::string filepath = path::formattedFullPath(directory, refined);

        if (webserv::path::fileExists(directory, refined)) {
            std::ofstream file(filepath.c_str(), std::ios_base::app);
            if (!file.good()) {
                resp->m_status = 500; // replace by generate error response
                resp->m_reason = resp->s_status_map[resp->m_status];
                return resp;
            } else {
                file << request->getContent();
                file.close();
                return resp;
            }
        } else {
            std::ofstream file(filepath.c_str(), std::ofstream::out | std::ofstream::trunc);
            if (!file.good()) {
                resp->m_status = 403; // replace by generate error response
                resp->m_reason = resp->s_status_map[resp->m_status];
                return resp;
            } else {
                file << request->getContent();
                file.close();
                return resp;
            }
        }
    } else {
        resp->m_status = 403; // Need to check this
        resp->m_reason = resp->s_status_map[resp->m_status];
        return resp;
    }

}

// smt::shared_ptr<HTTPResponse>
// HEAD(smt::shared_ptr<HTTPRequest>& request,
//                     smt::shared_ptr<LocationBlock>& location) {
//     // Check if method is allowed
//     bool allowed = location->m_allowed_methods.find("HEAD") != location->m_allowed_methods.end();

// }

smt::shared_ptr<HTTPResponse>
DELETE(smt::shared_ptr<HTTPRequest>& request,
                    smt::shared_ptr<LocationBlock>& location) {
    LOG_D("DELETE method called");
    std::string                        body;
    std::map<std::string, std::string> headers;
    smt::shared_ptr<webserv::HTTPResponse> resp;
    resp = smt::make_shared<webserv::HTTPResponse>(
        new webserv::HTTPResponse(200, headers, body));

    // Check if method is allowed
    bool allowed = location->m_allowed_methods.find("DELETE") != location->m_allowed_methods.end();
    if (!allowed) {
        resp->m_status = 403;
        resp->m_reason = resp->s_status_map[resp->m_status]; // Need to check this
        LOG_D("Returning error");
        return resp;
    }

    // Check if file in path and access is allowed (This logic is common to CGI)
    std::string refined = request->getRefinedResource();
    std::string directory = location->m_root; // FIXME: Shouldn't we have different roots for different methods

    LOG_D("Checking requested resource: " + path::formattedFullPath(directory, refined));
    if (!webserv::path::isCrawler(refined)) {
        std::string filepath = path::formattedFullPath(directory, refined);

        // Check if the file exists
        if (access(filepath.c_str(), F_OK) == -1) {
            // File not found
            resp->m_status = 404;
            resp->m_reason = resp->s_status_map[resp->m_status];
            LOG_D("RETURNING " + resp->m_status);
            return resp;
        }

        // Check if the file is a directory (directories cannot be deleted)
        struct stat file_stat;
        if (stat(filepath.c_str(), &file_stat) == -1) {
            resp->m_status = 500;
            resp->m_reason = resp->s_status_map[resp->m_status];
            LOG_D("RETURNING " + resp->m_status);
            return resp;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            resp->m_status = 405;
            resp->m_reason = resp->s_status_map[resp->m_status];
            LOG_D("RETURNING " + resp->m_status);
            return resp;
        }

        // Delete the file
        if (unlink(filepath.c_str()) == -1) {
            resp->m_status = 500;
            resp->m_reason = resp->s_status_map[resp->m_status];
            LOG_D("RETURNING " + resp->m_status);
            return resp;
        } else {
            resp->m_header["Content-Length"] = "0";
            LOG_D("RETURNING " + resp->m_status);
            return resp;
        }

    } else {
        resp->m_status = 403;
        resp->m_reason = resp->s_status_map[resp->m_status]; // Need to check this
        LOG_D("RETURNING " + resp->m_status);
        return resp;
    }
}

} // namespace webserv::methods