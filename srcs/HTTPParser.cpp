#include "HTTPParser.hpp"
#include "Logger.hpp"
#include <sstream>
#include <cstring>
#include <cmath>

HTTPParser::HTTPParser(){

}

HTTPParser::~HTTPParser(){
    
}

// A la golang, parsing functions return a pair of the actual result and a bool of success
std::pair<unsigned int, bool> HTTPParser::getMethod(std::string& firstLine){
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

std::pair<std::string, bool> HTTPParser::getResource(std::string& firstLine){
    std::istringstream iss(firstLine);
    std::string word;

    iss >> word;
    if (!(iss >> word)) {
        LOG_E("Only one word in first line of http request");
        return {"", false};
    }

    return {word, true};
}

std::pair<std::string, bool> HTTPParser::getVersion(std::string& firstLine){
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

std::pair<std::string, std::string> HTTPParser::getKeyValueHeader(std::string& header) {
    std::string first;
    std::string second;

    std::size_t sepIndex = header.find_first_of(":");
    if (sepIndex == std::string::npos) {
        LOG_E("No [key]:[value] found at header string: ->" + header + "<-");
        return {"",""};
    } else {
        first = header.substr(0, sepIndex);
        if (sepIndex < header.size())
            second = header.substr(sepIndex + 1, header.size());
        else
            second = "";
    }
    return {first, second};
}

bool HTTPParser::setHeader(smt::shared_ptr<HTTPRequest> pReq, std::string& header, std::string mustBe) {
    std::pair<std::string, std::string> result = getKeyValueHeader(header);
    if (result.first != mustBe) {
        LOG_E("Key didn't match " + mustBe + " (key is ->" + result.first + "<-)");
        return false;
    } else {
        pReq->headers[result.first] = result.second;
    }
    return true;
}

bool HTTPParser::setHeader(smt::shared_ptr<HTTPRequest> pReq, std::string& header) {
    std::pair<std::string, std::string> result = getKeyValueHeader(header);
    if (result.first == "") {
        return false;
    } else {
        pReq->headers[result.first] = result.second;
    }
    return true;
}

std::vector<std::string> HTTPParser::separateByCRLF(std::string& raw) {
	std::vector<std::string> separated;
    char crlf[3];

    crlf[0] = 13;
    crlf[1] = 10;
    crlf[2] = '\0';

    // Separate by \r\n
    size_t old = 0;
    size_t next = raw.find(crlf, old);
    int i = 5;
    while (next != std::string::npos && old <= next) {
        separated.push_back(raw.substr(old, next - old + 2));
        old = next + 2;
        next = raw.find(crlf, old);
        i--;
    }
    // Remove CRLF
    for (std::vector<std::string>::iterator it = separated.begin(); it != separated.end(); ++it){
        if ((*it).size() > 0) {
            (*it).resize((*it).size() - 2);
        }
    }
    return separated;
}

smt::shared_ptr<HTTPRequest> HTTPParser::parse_header(std::string& header) {
    std::vector<std::string> request = separateByCRLF(header);
	if (request.size() < 2){ // The method line /  and an empty line to signify end of headers
		LOG_E("HTTP Request has less than 2 lines");
        return NULL;
    }

    // Create a shared ptr -> this will delete itself if we dont return it :)
    HTTPRequest* dontUse = new HTTPRequest;
    smt::shared_ptr<HTTPRequest> pReq(dontUse);

    // Handle lines
    std::string& firstLine = request.at(0);
    std::string& secondLine = request.at(1);

    std::pair<unsigned int, bool> methodANDsuccess = getMethod(firstLine);
    if (!methodANDsuccess.second) return NULL;
    pReq->method = methodANDsuccess.first;

    std::pair<std::string, bool> resourceANDsuccess = getResource(firstLine);
    if (!resourceANDsuccess.second) return NULL;
    pReq->resource = resourceANDsuccess.first;

    std::pair<std::string, bool> versionANDsuccess = getVersion(firstLine);
    if (!versionANDsuccess.second) return NULL;
    pReq->version = versionANDsuccess.first;

    std::vector<std::string>::iterator it = request.begin();
    for (it = it + 1; it != request.end(); ++it) {
        if (*it != "") {
            if (!setHeader(pReq, *it)) {
                return NULL;
            }
        } else {
			break ; // Trying not to have it increment when it's a empty line
		}
    }
    if (it == request.end()) { // Means we didn't find an empty line
        LOG_E("No empty line signifying end of headers");
        return NULL;
    }
    return pReq;
}

int HTTPParser::find_next_request(const char* buff) const {
    std::string temp(buff);
                
    size_t zg = temp.find("GET", 0, 3);
    size_t zp = temp.find("POST", 0, 4);
    size_t zd = temp.find("DELETE", 0, 6);   

    size_t g = temp.find("\r\nGET ");
    size_t p = temp.find("\r\nPOST ");
    size_t d = temp.find("\r\nDELETE ");

    if (g == std::string::npos && p == std::string::npos && d == std::string::npos) {
        if (zg == std::string::npos && zp == std::string::npos && zd == std::string::npos) {
            return -1;
        }
    }

    return (std::min(g,
                std::min(d,
                    std::min(p,
                        std::min(zg,
                            std::min(zp, zd))))));
}
