#include "http/HTTPParser.hpp"

namespace webserv {

HTTPParser::HTTPParser() {}

HTTPParser::~HTTPParser() {}

// A la golang, parsing functions return a pair of the actual result and a bool
// of success
std::pair<webserv::Method, bool> HTTPParser::getMethod(std::string& firstLine) {
    if (firstLine.empty()) { return {webserv::UNDEFINED, false}; }

    // Getting first word from line
    std::istringstream iss(firstLine);
    std::string        firstWord;

    iss >> firstWord;
    if (firstWord == "GET") return {webserv::GET, true};
    else if (firstWord == "POST") return {webserv::POST, true};
    else if (firstWord == "DELETE") return {webserv::DELETE, true};
    else return {webserv::UNDEFINED, false};
}

std::pair<std::string, bool> HTTPParser::getResource(std::string& firstLine) {
    std::istringstream iss(firstLine);
    std::string        word;

    iss >> word;
    if (!(iss >> word)) {
        LOG_E("Only one word in first line of http request");
        return {"", false};
    }

    return {word, true};
}

std::pair<std::string, bool> HTTPParser::getVersion(std::string& firstLine) {
    std::istringstream iss(firstLine);
    std::string        word;

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

std::pair<std::string, std::string>
    HTTPParser::getKeyValueHeader(std::string& header) {
    std::string first;
    std::string second;

    std::size_t sepIndex = header.find_first_of(":");
    if (sepIndex == std::string::npos) {
        LOG_E("No [key]:[value] found at header string: ->" + header + "<-");
        return {"", ""};
    }
    else {
        first = header.substr(0, sepIndex);
        if (sepIndex < header.size())
            second = header.substr(sepIndex + 1, header.size());
        else second = "";
    }
    return {first, second};
}

bool HTTPParser::setHeader(smt::shared_ptr<HTTPRequest> pReq,
                           std::string& header, std::string mustBe) {

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
    // HTTPRequest* dontUse = new HTTPRequest;
    // smt::shared_ptr<HTTPRequest> pReq(dontUse);
    smt::shared_ptr<HTTPRequest> pReq = smt::make_shared<HTTPRequest>();

    std::vector<std::string> request = separateByCRLF(header);
    if (request.size() < 2) {
        // The method line /  and an empty line to signify end of headers
        LOG_E("HTTP Request has less than 2 lines");
        return (smt::make_shared<HTTPRequest>(42));
    }

    // Handle lines
    std::string& firstLine = request.at(0);
    std::string& secondLine = request.at(1);

    std::pair<webserv::Method, bool> methodANDsuccess = getMethod(firstLine);
    if (!methodANDsuccess.second) return (smt::make_shared<HTTPRequest>(42));
    pReq->setMethod(methodANDsuccess.first);

    std::pair<std::string, bool> resourceANDsuccess = getResource(firstLine);
    if (!resourceANDsuccess.second) return (smt::make_shared<HTTPRequest>(42));
    pReq->setResource(resourceANDsuccess.first);

    std::pair<std::string, bool> versionANDsuccess = getVersion(firstLine);
    if (!versionANDsuccess.second) return (smt::make_shared<HTTPRequest>(42));
    pReq->setVersion(versionANDsuccess.first);

    std::vector<std::string>::iterator it = request.begin();
    for (it = it + 1; it != request.end(); ++it) {
        if (*it != "") {
            if (!setHeader(pReq, *it)) {
                return (smt::make_shared<HTTPRequest>(42));
            }
        }
        else {
            // Trying not to have it increment when it's a empty line
            break;
        }
    }
    if (it == request.end()) { // Means we didn't find an empty line

        LOG_E("No empty line signifying end of headers");
        return (smt::make_shared<HTTPRequest>(42));
    }
    return (pReq);
}

// SYSCAL rcv triggers handler -> handler will keep calling getNextRequest()
// until no request is returned
smt::shared_ptr<HTTPRequest> HTTPParser::getNextRequest(std::string received) {

    // When recv is called it returns an HTTP request
    smt::shared_ptr<HTTPRequest> request;

    // Declare variables
    std::string        data;
    static std::string restOfData;    // Buffer for storing data left over from
                                      // previous call to recv
    static bool dataInBuffer = false; // Flag to indicate whether there is data
                                      // left over in the restOfData buffer
    size_t bytes_read = 0;            // Number of bytes read from the socket

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
        if (received.empty()) { return (smt::make_shared<HTTPRequest>(42)); }
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
            return (smt::make_shared<HTTPRequest>(413));
        }
        // Store what we've gotten
        restOfData = data;
        dataInBuffer = true;
        return (smt::make_shared<HTTPRequest>(42)); // we need more data
    }
    else {

        eoh_position += 4;
        header = data.substr(0, eoh_position);
        header.insert(eoh_position, "");
    }
    // Pass the headers to the parser and receive a "valid" HTTPRequest
    request = parse_header(header);
    if (!request->isValid()) { return (request); }

    // If there is more data to be dealt with
    if (data.length() > eoh_position) {

        // If we have a Content-Length
        std::string lenStr = request->getHeader("Content-Length");
        if (!lenStr.empty()) {

            int body_size =
                std::min(atoi(lenStr.c_str()),
                         static_cast<int>(data.length() - (eoh_position)));

            if (body_size < MAX_BODY_SIZE && body_size) {

                int restOfBuff = data.length() - (eoh_position);
                request->setContent(
                    data.substr(eoh_position, std::min(restOfBuff, body_size)));

                // If there's more to the body then what we have
                if (body_size > request->getContent().size()) {
                    // Store what we've gotten
                    restOfData = data;
                    dataInBuffer = true;
                    // we need more data
                    return (smt::make_shared<HTTPRequest>(42));
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

    return (request);
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

// // SYSCAL rcv triggers handler -> handler will keep calling getNextRequest()
// until no request is returned smt::shared_ptr<HTTPRequest>
// HTTPParser::getNextRequest(std::string received) {
//     // When recv is called it returns an HTTP request
//     smt::shared_ptr<HTTPRequest> request;
//
//     // Declare variables
//     char            buff[READING_BUFFER + 1]; // Buffer for reading data from
//     socket static char     restOfData[READING_BUFFER + 1]; // Buffer for
//     storing data left over from previous call to recv static bool
//     dataInBuffer = false; // Flag to indicate whether there is data left over
//     in the restOfData buffer size_t          bytes_read = 0; // Number of
//     bytes read from the socket
//
//     char*       endOfHeaders = "\r\n\r\n"; // String to identify the end of
//     the headers in the request size_t      eoh_position; // Position of the
//     end of the headers in the buffer
//
//     // String to store the request headers
//     std::string header;
//     header.reserve(READING_BUFFER + 1); // Pre-allocate memory for the string
//     to improve performance
//
//     // If there is no data left over from a previous call to recv
//     if (!dataInBuffer) {
//         // Get data from connection
//         // bytes_read = ::recv(fd, &buff, READING_BUFFER, 0);
//         // if (bytes_read <= 0) {
//         //     LOG_I("No data in connection");
//         //     return NULL;
//         // }
//         // buff[bytes_read] = '\0';
//         if (received.empty()) {
//             return NULL;
//         }
//     } else { // Means we don't have to make a call to receive
//         data_to_buff(restOfData, buff);
//         dataInBuffer = false;
//     }
//
//     // Get headers from data
//     header = buff;
//     if ((eoh_position = header.find(endOfHeaders, 0)) == std::string::npos) {
//         LOG_I("More than 8k header size");
//         return NULL; // but actually return 413 Entity Too Large
//     } else {
//         eoh_position += 4;
//         header.insert(eoh_position, "");
//     }
//
//     // Pass the headers to the parser and receive a "valid" HTTPRequest
//     request = parser.parse_header(header);
//     if (!request) {
//         return NULL;
//     }
//
//     // If there is more data to be dealt with
//     if (strlen(buff) > eoh_position) {
//         // If we have a Content-Lenght
//         std::string lenStr = request->getHeader("Content-Length");
//         if (!lenStr.empty()) {
//             int body_size = std::min(atoi(lenStr.c_str()),
//             static_cast<int>(strlen(buff) - (eoh_position))); if (body_size <
//             MAX_BODY_SIZE && body_size) {
//                 // request->getContent().reserve(body_size);
//                 std::string temp(buff);
//
//                 int restOfBuff = temp.size() - (eoh_position);
//                 request->setContent(temp.substr(eoh_position,
//                 std::min(restOfBuff, body_size)));
//
//                 while (body_size > request->getContent().size()) { // While
//                 there's more to the body then what we have
//                     while (true){
//                         bytes_read = ::recv(fd, &buff, READING_BUFFER, 0);
//                         if (bytes_read < 0) {
//                             return NULL;
//                             LOG_E("Error when reading from socket");
//                         } else if (bytes_read > 0) {
//                             buff[bytes_read] = '\0';
//                             break;
//                         }
//                     }
//                     temp = buff;
//                     // What I want to do here is add what's missing until we
//                     are of size: body_size int chars_still_missing =
//                     body_size - (request->getContent().length()); // maybe +1
//                     for the NTC? request->setContent(request->getContent() +
//                     temp.substr(0, std::min(static_cast<int>(temp.length()),
//                     chars_still_missing)));
//                 }
//             }
//         }
//
//         // Clear what we have read from buffer (and store the rest in buff)
//         std::string formatter(buff);
//         std::string formatted(formatter.substr(eoh_position +
//         request->getContent().length(), strlen(buff)).c_str()); const char
//         *writer = formatted.c_str(); int index; for (index = 0; writer[index]
//         != '\0'; ++index) {
//             buff[index] = writer[index];
//         }
//         buff[index] = '\0';
//
//         // Ignore the rest of info up until next request
//         int next = parser.find_next_request(buff);
//         if (next == -1) {
//             dataInBuffer = false;
//         } else {
//             std::string temp(buff);
//             std::string store = temp.substr(next);
//             buff_to_data(const_cast<char *>(store.c_str()), restOfData);
//             dataInBuffer = true;
//         }
//     }
//
//     return request;
// }

} // namespace webserv
