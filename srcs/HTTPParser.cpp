#include "HTTPParser.hpp"
#include "Logger.hpp"
#include <sstream>
#include <cstring>


// A la golang, parsing functions return a pair of the actual result and a bool of success
std::pair<unsigned int, bool> getMethod(std::string& firstLine){
    if (firstLine.empty()) {
        return {0, false};
    }

    // Getting first word from line
    std::istringstream iss(firstLine);
    std::string firstWord;

    iss >> firstWord;
    if (firstWord == "GET") return {1, true};
    else if (firstWord == "POST") return {2, true};
    else if (firstWord == "DELETE") return {3, true};
    else return {0, false};
}

std::pair<std::string, bool> getResource(std::string& firstLine){
    std::istringstream iss(firstLine);
    std::string word;

    iss >> word;
    if (!(iss >> word)) {
        LOG_E("Only one word in first line of http request");
        return {"", false};
    }

    return {word, true};
}

std::pair<std::string, bool> getVersion(std::string& firstLine){
    std::istringstream iss(firstLine);
    std::string word;

    iss >> word;
    if (!(iss >> word)) {
        LOG_E("Only one word in first line of http request");
        return {"", false};
    }
    if (!(iss >> word)) {
        LOG_E("Only two words in first line of http request");
        return {"", false};
    }
    return {word, true};
}

smt::shared_ptr<HTTPRequest> HTTPParser::parse(std::vector<std::string>& request) {
	if (request.empty()){
        return NULL;
    }

    // Create a shared ptr -> this will delete itself if we dont return it :)
    HTTPRequest* dontUse = new HTTPRequest;
    smt::shared_ptr<HTTPRequest> pReq(dontUse);

    // Handle first line
    std::string& firstLine = request.at(0);

    std::pair<unsigned int, bool> methodANDsuccess = getMethod(firstLine);
    if (!methodANDsuccess.second) return NULL;
    pReq->method = methodANDsuccess.first;

    std::pair<std::string, bool> resourceANDsuccess = getResource(firstLine);
    if (!resourceANDsuccess.second) return NULL;
    pReq->resource = resourceANDsuccess.first;

    std::pair<std::string, bool> versionANDsuccess = getVersion(firstLine);
    if (!versionANDsuccess.second) return NULL;
    pReq->version = versionANDsuccess.first;

    return pReq;
}
