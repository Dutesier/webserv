#ifndef HTTPPARSER_HPP
#define HTTPPARSER_HPP

#include "HTTPRequest.hpp"
#include "utils/Logger.hpp"
#include "utils/smt.hpp"
#include "utils/utils.hpp"

#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifndef MAX_HEADER_SIZE
# define MAX_HEADER_SIZE 8192
#endif

class HTTPParser {
    public:

        HTTPParser();
        ~HTTPParser();

        std::pair<smt::shared_ptr<HTTPRequest>, bool>
            getNextRequest(std::string received);

        void setMaxBodySize(unsigned int maxBodySize);

    private:

        std::pair<webserv::Method, bool> getMethod(std::string& firstLine);
        std::pair<std::string, bool>     getResource(std::string& firstLine);
        std::pair<std::string, bool>     getVersion(std::string& firstLine);
        std::pair<std::string, std::string>
             getKeyValueHeader(std::string& header);
        bool setHeader(smt::shared_ptr<HTTPRequest> pReq, std::string& header);
        bool setHeader(smt::shared_ptr<HTTPRequest> pReq, std::string& header,
                       std::string mustBe);
        std::vector<std::string>     separateByCRLF(std::string& raw);
        smt::shared_ptr<HTTPRequest> parse_header(std::string& header);
        int                          find_next_request(std::string& buff) const;

    private:

        // Buffer for storing data left over from
        // previous call to recv
        std::string restOfData;

        // Flag to indicate whether there is data
        // left over in the restOfData buffer
        bool dataInBuffer;

        // Configurable max body size
        unsigned int m_max_body_size;
};

#endif /* HTTPPARSER_HPP */
