#include "http/HTTPParser.hpp"

#include <cstdlib>

HTTPParser::HTTPParser() : m_max_body_size(MAX_BODY_SIZE) {}

HTTPParser::~HTTPParser() {}

// A la golang, parsing functions return a pair of the actual result and a bool
// of success
std::pair<webserv::Method, bool> HTTPParser::getMethod(std::string& firstLine) {
    if (firstLine.empty()) { return std::make_pair(webserv::UNDEFINED, false); }

    // Getting first word from line
    std::istringstream iss(firstLine);
    std::string        firstWord;

    iss >> firstWord;
    if (firstWord == "GET") return std::make_pair(webserv::GET, true);
    else if (firstWord == "POST") return std::make_pair(webserv::POST, true);
    else if (firstWord == "DELETE")
        return std::make_pair(webserv::DELETE, true);
    else return std::make_pair(webserv::UNDEFINED, false);
}

std::pair<std::string, bool> HTTPParser::getResource(std::string& firstLine) {
    std::istringstream iss(firstLine);
    std::string        word;

    iss >> word;
    if (!(iss >> word)) {
        LOG_E("Only one word in first line of http request");
        return std::make_pair("", false);
    }

    return std::make_pair(word, true);
}

std::pair<std::string, bool> HTTPParser::getVersion(std::string& firstLine) {
    std::istringstream iss(firstLine);
    std::string        word;

    iss >> word;
    if (!(iss >> word)) {
        LOG_E("Only one word in first line of http request");
        return std::make_pair("", false);
    }
    if (!(iss >> word)) {
        LOG_E("Only two words in first line of http request");
        return std::make_pair("", false);
    }
    return std::make_pair(word, true);
}

std::pair<std::string, std::string>
    HTTPParser::getKeyValueHeader(std::string& header) {
    std::string first;
    std::string second;

    std::size_t sepIndex = header.find_first_of(":");
    if (sepIndex == std::string::npos) {
        LOG_E("No [key]:[value] found at header string: ->" + header + "<-");
        return std::make_pair("", "");
    }
    else {
        first = header.substr(0, sepIndex);
        if (sepIndex < header.size())
            second = header.substr(sepIndex + 1, header.size());
        else second = "";
    }
    return std::make_pair(first, second);
}

bool HTTPParser::setHeader(smt::shared_ptr<HTTPRequest> pReq,
                           std::string& header, std::string mustBe) {

    std::cout << "Setting header " << header << " with mustBe " << mustBe
              << std::endl;
    std::pair<std::string, std::string> result = getKeyValueHeader(header);
    if (result.first != mustBe) {

        LOG_E("Key didn't match " + mustBe + " (key is ->" + result.first +
              "<-)");
        return (false);
    }
    else { pReq->setHeader(result.first, result.second); }
    return (true);
}

bool HTTPParser::setHeader(smt::shared_ptr<HTTPRequest> pReq,
                           std::string&                 header) {

    std::pair<std::string, std::string> result = getKeyValueHeader(header);
    if (result.first == "") { return (false); }
    else { pReq->setHeader(result.first, result.second); }
    return (true);
}

std::vector<std::string> HTTPParser::separateByCRLF(std::string& raw) {

    std::vector<std::string> separated;
    char                     crlf[3];

    crlf[0] = 13;
    crlf[1] = 10;
    crlf[2] = '\0';

    // Separate by \r\n
    size_t old = 0;
    size_t next = raw.find(crlf, old);
    int    i = 5;
    while (next != std::string::npos && old <= next) {

        separated.push_back(raw.substr(old, next - old + 2));
        old = next + 2;
        next = raw.find(crlf, old);
        i--;
    }
    // Remove CRLF
    for (std::vector<std::string>::iterator it = separated.begin();
         it != separated.end(); ++it) {

        if ((*it).size() > 0) { (*it).resize((*it).size() - 2); }
    }
    return separated;
}

smt::shared_ptr<HTTPRequest> HTTPParser::parse_header(std::string& header) {

    // Create a shared ptr -> this will delete itself if we dont return it :)
    smt::shared_ptr<HTTPRequest> pReq = smt::make_shared(new HTTPRequest);

    std::vector<std::string> request = separateByCRLF(header);
    if (request.size() < 2) {
        // The method line / and an empty line to signify end of headers
        LOG_E("HTTP Request has less than 2 lines");
        return (smt::make_shared(new HTTPRequest(42)));
    }

    // Handle lines
    std::string& firstLine = request.at(0);

    std::pair<webserv::Method, bool> methodANDsuccess = getMethod(firstLine);
    if (!methodANDsuccess.second)
        return (smt::make_shared(new HTTPRequest(42)));
    pReq->setMethod(methodANDsuccess.first);

    std::pair<std::string, bool> resourceANDsuccess = getResource(firstLine);
    if (!resourceANDsuccess.second)
        return (smt::make_shared(new HTTPRequest(42)));
    pReq->setResource(resourceANDsuccess.first);

    std::pair<std::string, bool> versionANDsuccess = getVersion(firstLine);
    if (!versionANDsuccess.second)
        return (smt::make_shared(new HTTPRequest(42)));
    pReq->setVersion(versionANDsuccess.first);

    std::vector<std::string>::iterator it = request.begin();
    for (it = it + 1; it != request.end(); ++it) {
        if (*it != "") {
            if (!setHeader(pReq, *it)) {
                return (smt::make_shared(new HTTPRequest(42)));
            }
        }
        else {
            // Trying not to have it increment when it's a empty line
            break;
        }
    }
    if (it == request.end()) { // Means we didn't find an empty line

        LOG_E("No empty line signifying end of headers");
        return (smt::make_shared(new HTTPRequest(42)));
    }
    return (pReq);
}

// SYSCAL rcv triggers handler -> handler will keep calling getNextRequest()
// until no request is returned
// bool in pair is "keep asking for requests"
std::pair<smt::shared_ptr<HTTPRequest>, bool>
    HTTPParser::getNextRequest(std::string received) {
    // When recv is called it returns an HTTP request
    smt::shared_ptr<HTTPRequest> request;

    // Declare variables
    std::string data;

    // String to identify the end of the headers in the request
    char const* endOfHeaders = "\r\n\r\n";
    // Position of the end of the headers in the buffer
    size_t eoh_position;

    // String to store the request headers
    std::string header;
    // Pre-allocate memory for the string to improve performance
    header.reserve(MAX_HEADER_SIZE + 1);

    // If there is no data left over from a previous call to recv
    if (!dataInBuffer) {
        // and if we didn't get anything now
        if (received.empty()) {
            return (
                std::make_pair(smt::make_shared(new HTTPRequest(42)), false));
        }
        else { data = received; }
    }
    else { // There is still data in buffer

        data = restOfData;
        if (!received.empty()) { data += received; }
        restOfData = "";
        dataInBuffer = false;
    }

    // Get headers from data
    // header = data.substr(0, );
    if ((eoh_position = data.find(endOfHeaders, 0)) == std::string::npos) {
        // Means we couldn't find the end of headers (Maybe
        // its in another RECV or maybe this is just ill
        // formed)
        if (data.length() >= MAX_HEADER_SIZE) {

            LOG_I("More than 8k header size");
            // but actually return 413 Entity Too Large
            return (
                std::make_pair(smt::make_shared(new HTTPRequest(413)), false));
        }
        // Store what we've gotten
        restOfData = data;
        dataInBuffer = true;
        LOG_W("No end of headers found");
        return (std::make_pair(smt::make_shared(new HTTPRequest(42)), false));
    }
    else {
        // We found the end of headers
        eoh_position += 4;
        header = data.substr(0, eoh_position);
        header.insert(eoh_position, "");
    }
    // Pass the headers to the parser and receive a "valid" HTTPRequest
    request = parse_header(header);
    if (!request->isValid()) { return std::make_pair(request, true); }

    // If there is more data to be dealt with
    if (data.length() > eoh_position) {

        // If we have a Content-Length
        std::string lenStr = request->getHeader("Content-Length");
        if (!lenStr.empty()) {

            long unsigned     size;
            std::stringstream ss(lenStr);
            ss >> size;

            long unsigned body_size =
                std::min(size, static_cast<long unsigned>(data.length() -
                                                          (eoh_position)));

            if (body_size < m_max_body_size && body_size) {

                long unsigned restOfBuff = data.length() - (eoh_position);
                request->setContent(
                    data.substr(eoh_position, std::min(restOfBuff, body_size)));

                // If there's more to the body then what we have
                if (body_size > request->getContent().size()) {
                    // Store what we've gotten
                    restOfData = data;
                    dataInBuffer = true;
                    // we need more data to be written in the FD
                    LOG_W("Missing body content");
                    return (std::make_pair(
                        smt::make_shared(new HTTPRequest(42)), false));
                }
            }
        }

        // Ignore the rest of info up until next request (If we've gotten here
        // is because we've successfully parsed a Request)
        int requestSize = eoh_position + request->getContent().length();
        data = data.substr(requestSize); // From requestSize onwards
        int next = find_next_request(data);
        // No http request in data
        if (next == -1) { dataInBuffer = false; }
        else {

            restOfData = data.substr(next);
            dataInBuffer = true;
        }
    }

    return std::make_pair(request, true);
}

int HTTPParser::find_next_request(std::string& buff) const {

    std::string temp(buff);

    size_t zg = temp.find("GET", 0, 3);
    size_t zp = temp.find("POST", 0, 4);
    size_t zd = temp.find("DELETE", 0, 6);

    size_t g = temp.find("\r\nGET ");
    size_t p = temp.find("\r\nPOST ");
    size_t d = temp.find("\r\nDELETE ");

    if (g == std::string::npos && p == std::string::npos &&
        d == std::string::npos) {
        if (zg == std::string::npos && zp == std::string::npos &&
            zd == std::string::npos) {
            return (-1);
        }
    }

    return (
        std::min(g, std::min(d, std::min(p, std::min(zg, std::min(zp, zd))))));
}

void HTTPParser::setMaxBodySize(unsigned int maxBodySize) {
    m_max_body_size = maxBodySize;
}
