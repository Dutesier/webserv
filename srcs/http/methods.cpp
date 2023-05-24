#include "http/methods.hpp"

#include "http/HTTPHandler.hpp"
#include "http/pathValidation.hpp"

#include <dirent.h>
#include <fstream>
#include <sstream>
#include <unistd.h>

namespace webserv::methods {

bool readContentsFromFileToResponse(std::string const&             filepath,
                                    smt::shared_ptr<HTTPResponse>& response) {

    std::ifstream     file(filepath.c_str());
    std::string       line;
    std::stringstream ss;
    std::stringstream responseBody;

    // Exit if file is not good
    if (!file.good()) {
        LOG_E("Error: File " + filepath + " is not good.");
        response->m_status = 500;
        response->m_reason = response->s_status_map[response->m_status];
        return false;
    }

    // Write buffer to HTTPResponse
    responseBody << file.rdbuf();
    response->m_body = responseBody.str();
    ss << (response->m_body.length());
    response->m_header["Content-Length"] = ss.str();
    file.close();
    return true;
}

std::string generateAutoIndex(std::string directoryPath) {
    // Check if the directory exists
    DIR* dir = opendir(directoryPath.c_str());
    if (!dir) {
        LOG_E("Error: Directory does not exist.");
        return "";
    }
    closedir(dir);

    // Get the list of files in the directory
    std::stringstream ss;
    ss << "<html><head><title>Index of " << directoryPath
       << "</title></head><body>";
    ss << "<h1>Index of " << directoryPath << "</h1>";
    ss << "<ul>";

    dirent* entry;
    dir = opendir(directoryPath.c_str());
    while ((entry = readdir(dir)) != NULL) {
        struct stat stat;
        if (entry->d_name[0] != '.') {
            if (lstat(std::string(directoryPath.c_str() +
                                  std::string(entry->d_name))
                          .c_str(),
                      &stat) != -1 &&
                S_ISDIR(stat.st_mode)) {
                ss << "<li><a href=\"" << std::string(entry->d_name) << "/\"/>"
                   << std::string(entry->d_name) << "/</a></li>\n";
            }
            else {
                ss << "<li><a href=\"" << std::string(entry->d_name) << "\"/>"
                   << std::string(entry->d_name) << "</a></li>\n";
            }
        }
    }
    closedir(dir);

    ss << "</ul>";
    ss << "</body></html>";

    return ss.str();
}

smt::shared_ptr<HTTPResponse> GET(smt::shared_ptr<HTTPRequest>&   request,
                                  smt::shared_ptr<LocationBlock>& location) {
    LOG_D("GET method called");
    std::string                            body;
    std::map<std::string, std::string>     headers;
    smt::shared_ptr<webserv::HTTPResponse> resp;

    // Create a basic HTTPResponse to be populated during the method
    resp = smt::make_shared<webserv::HTTPResponse>(
        new webserv::HTTPResponse(200, headers, body));

    // Check if method is allowed
    bool allowed = location->m_allowed_methods.find("GET") !=
                   location->m_allowed_methods.end();
    if (!allowed) {
        resp->m_status = 403; // Need to check this
        resp->m_reason = resp->s_status_map[resp->m_status];
        return resp;
    }

    // Check if file in path and access is allowed (This logic is common to CGI)
    std::string refined = request->getRefinedResource();
    std::string directory =
        location->m_root; // NOTE: If I have the correct location here then HTTP
                          // redirection is done!
    std::string filepath = path::formattedFullPath(directory, refined);

    // Exit if file is a crawler
    if (webserv::path::isCrawler(refined)) {
        resp->m_status = 403; // FIXME: Check correct status code
        resp->m_reason = resp->s_status_map[resp->m_status];
        return resp;
    }

    // Exit if file does not exist
    if (!webserv::path::fileExists(filepath)) {
        resp->m_status = 404; // FIXME: Check correct status code
        resp->m_reason = resp->s_status_map[resp->m_status];
        return resp;
    }

    // Handle the case where the file is a directory
    if (webserv::path::isDirectory(filepath)) {
        // If index file has a value we should return the contents of index file
        if (location->m_index != "") {
            // Read contents from file to HTTPResponse
            if (!readContentsFromFileToResponse(
                    webserv::path::formattedFullPath(directory,
                                                     location->m_index),
                    resp)) {
                // If auto index is on we should return 404
                if (location->m_autoindex) { resp->m_status = 404; }
                // If auto index is off we should return 403
                if (!location->m_autoindex) { resp->m_status = 403; }
                resp->m_reason = resp->s_status_map[resp->m_status];
                return resp;
            }

            // If we're here means we have successfully read the contents of
            // index
            return resp;
        }

        // If auto index is off and index file is not set we should return 403
        // Check if autoindex is not on
        if (location->m_autoindex == false) { // Waiting on
                                              // implementation
            resp->m_status = 403; // FIXME: Check correct status code
            resp->m_reason = resp->s_status_map[resp->m_status];
            return resp;
        }

        // Generate autoindex
        std::string autoIndex = generateAutoIndex(filepath);
        // Exit if autoindex generation fails
        if (autoIndex == "") {
            resp->m_status = 403; // FIXME: Check correct status code
            resp->m_reason = resp->s_status_map[resp->m_status];
            return resp;
        }

        // Populate HTTPResponse
        resp->m_body = autoIndex;
        resp->m_header["Content-Length"] = std::to_string(autoIndex.length());
        return resp;
    }

    // Handle the case where it's a regular file
    readContentsFromFileToResponse(filepath, resp);
    // Return HTTPResponse
    return resp;
}

smt::shared_ptr<HTTPResponse> POST(smt::shared_ptr<HTTPRequest>&   request,
                                   smt::shared_ptr<LocationBlock>& location) {
    LOG_D("POST method called");
    std::string                            body;
    std::map<std::string, std::string>     headers;
    smt::shared_ptr<webserv::HTTPResponse> resp;
    resp = smt::make_shared<webserv::HTTPResponse>(
        new webserv::HTTPResponse(200, headers, body));

    // Check if method is allowed
    bool allowed = location->m_allowed_methods.find("POST") !=
                   location->m_allowed_methods.end();
    if (!allowed) {
        resp->m_status = 403; // Need to check this
        resp->m_reason = resp->s_status_map[resp->m_status];
        LOG_D("Returning error");
        return resp;
    }

    // Check if file in path and access is allowed (This logic is common to CGI)
    std::string refined = request->getRefinedResource();
    std::string directory =
        location->m_root; // NOTE: If I have the correct location here then HTTP
                          // redirection is done!
    std::string filepath = path::formattedFullPath(directory, refined);

    // Exit if file is a crawler
    if (webserv::path::isCrawler(refined)) {
        resp->m_status = 403; // FIXME: Check correct status code
        resp->m_reason = resp->s_status_map[resp->m_status];
        return resp;
    }

    // Append to file if it exists
    if (webserv::path::fileExists(filepath)) {
        std::ofstream file(filepath.c_str(), std::ios_base::app);
        // Exit if file is not good
        if (!file.good()) {
            resp->m_status = 500;
            resp->m_reason = resp->s_status_map[resp->m_status];
            return resp;
        }
        // Write contents to file
        file << request->getContent();
        file.close();
        // Return sucess response
        return resp;
    }

    // Create a new file
    std::ofstream file(filepath.c_str(),
                       std::ofstream::out | std::ofstream::trunc);
    // Exit if file is not good
    if (!file.good()) {
        resp->m_status = 403; // replace by generate error response
        resp->m_reason = resp->s_status_map[resp->m_status];
        return resp;
    }
    // Write contents to file
    file << request->getContent();
    file.close();
    // Return sucess response
    return resp;
}

smt::shared_ptr<HTTPResponse> DELETE(smt::shared_ptr<HTTPRequest>&   request,
                                     smt::shared_ptr<LocationBlock>& location) {
    LOG_D("DELETE method called");
    std::string                            body;
    std::map<std::string, std::string>     headers;
    smt::shared_ptr<webserv::HTTPResponse> resp;
    resp = smt::make_shared<webserv::HTTPResponse>(
        new webserv::HTTPResponse(200, headers, body));

    // Check if method is allowed
    bool allowed = location->m_allowed_methods.find("DELETE") !=
                   location->m_allowed_methods.end();
    if (!allowed) {
        resp->m_status = 403;
        resp->m_reason =
            resp->s_status_map[resp->m_status]; // Need to check this
        LOG_D("Returning error");
        return resp;
    }

    // Check if file in path and access is allowed (This logic is common to CGI)
    std::string refined = request->getRefinedResource();
    std::string directory =
        location->m_root; // NOTE: If I have the correct location here then HTTP
                          // redirection is done!
    std::string filepath = path::formattedFullPath(directory, refined);

    // Exit if file is a crawler
    if (webserv::path::isCrawler(refined)) {
        resp->m_status = 403;
        resp->m_reason =
            resp->s_status_map[resp->m_status]; // Need to check this
        return resp;
    }

    // Exit if the file does not exist
    if (!webserv::path::fileExists(filepath)) {
        resp->m_status = 404;
        resp->m_reason = resp->s_status_map[resp->m_status];
        return resp;
    }

    // Exit if the file is a directory
    if (webserv::path::isDirectory(filepath)) {
        resp->m_status = 403;
        resp->m_reason = resp->s_status_map[resp->m_status];
        return resp;
    }

    // Delete the file
    if (unlink(filepath.c_str()) == -1) {
        resp->m_status = 500;
        resp->m_reason = resp->s_status_map[resp->m_status];
        return resp;
    }
    else {
        resp->m_header["Content-Length"] = "0";
        return resp;
    }
}

} // namespace webserv::methods