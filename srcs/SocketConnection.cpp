#include "SocketConnection.hpp"
#include <cstring>
#include <cstdlib>
#include "Logger.hpp"

namespace webserv {

SocketConnection::SocketConnection(int fd, SocketAddress addr)
    : Socket(fd, addr, SOCK_STREAM) {
    FLOG_D("webserv::SocketConnection created a socket");
}

SocketConnection::SocketConnection() : Socket() {
    FLOG_D("webserv::SocketConnection created a socket");
}

SocketConnection::~SocketConnection(void) {
    FLOG_D("webserv::SocketConnection destroyed a socket");
    this->close();
}

void SocketConnection::close(void) {
    if (this->fd == -1) return;
    if (::close(this->fd) < 0) throw(CloseFailureException());
    this->fd = -1;
    FLOG_D("webserv::SocketConnection closed a socket");
}

void data_to_buff(char* data, char *buff) {
    for (int i = 0; i < READING_BUFFER; ++i) {
        buff[i] = data[i];
        data[i] = 0;
    }
    buff[READING_BUFFER] = '\0';
}

void buff_to_data(char* buff, char* data) {
    for (int i = 0; buff[i] != '\0'; ++i){
        data[i] = buff[i];
    }
}
// TODO: find smarter ways to get buf
smt::shared_ptr<HTTPRequest> SocketConnection::recv(void) { // When recv is called it returns an http request
    smt::shared_ptr<HTTPRequest> request;

    char            buff[READING_BUFFER + 1];
    static char     restOfData[READING_BUFFER + 1]; // Should always start with an HTTP RequestLine
    static bool     dataInBuffer = false; // Will be true when there was a previous call to recv that got its HTTP request and the beggining of ours
    size_t          bytes_read = 0;
    
    char*       endOfHeaders = "\r\n\r\n";
    size_t      eoh_position;
    
    std::string header;
    header.reserve(READING_BUFFER + 1);

    if (!dataInBuffer) { 
        // Get data from FD
        bytes_read = ::recv(fd, &buff, READING_BUFFER, 0);
        if (bytes_read <= 0) {
            LOG_I("No data in connection");
            return NULL;
        }
        buff[bytes_read] = '\0';
    } else { // Means we don't have to make a call to receive
        data_to_buff(restOfData, buff);
        dataInBuffer = false;
    }

    // Get headers from data
    header = buff;
    if ((eoh_position = header.find(endOfHeaders, 0)) == std::string::npos) {
        LOG_I("More than 8k header size");
        return NULL; // but actually return 413 Entity Too Large
    } else {
        eoh_position += 4;
        header.insert(eoh_position, "");
    }
    request = parser.parse_header(header);

    // If there is more data to be dealt with
    if (strlen(buff) > eoh_position) {
        // If we have a Content-Lenght
        if (request->headers.find("Content-Length") != request->headers.end()) {
            int body_size = std::min(atoi(request->headers.find("Content-Length")->second.c_str()), static_cast<int>(strlen(buff) - (eoh_position)));
            if (body_size < MAX_BODY_SIZE && body_size) {
                request->content.reserve(body_size);
                std::string temp(buff);

                int restOfBuff = temp.size() - (eoh_position);
                request->content = temp.substr(eoh_position, std::min(restOfBuff, body_size));

                while (body_size > request->content.size()) { // While there's more to the body then what we have
                    while (true){
                        bytes_read = ::recv(fd, &buff, READING_BUFFER, 0);
                        if (bytes_read < 0) {
                            return NULL;
                            LOG_E("Error when reading from socket");
                        } else if (bytes_read > 0) {
                            buff[bytes_read] = '\0';
                            break;
                        }
                    }
                    temp = buff;
                    // What I want to do here is add what's missing until we are of size: body_size
                    int chars_still_missing = body_size - (request->content.length()); // maybe +1 for the NTC?
                    request->content += temp.substr(0, std::min(static_cast<int>(temp.length()), chars_still_missing));
                }
            }
        }

        // Clear what we have read from buffer (and store the rest in buff)
        std::string formatter(buff);
        std::string formatted(formatter.substr(eoh_position + request->content.length(), strlen(buff)).c_str());
        const char *writer = formatted.c_str();
        int index;
        for (index = 0; writer[index] != '\0'; ++index) {
            buff[index] = writer[index];
        }
        buff[index] = '\0';

        // Ignore the rest of info up until next request
        int next = parser.find_next_request(buff);
        if (next == -1) {
            dataInBuffer = false;
        } else {
            std::string temp(buff);
            std::string store = temp.substr(next);
            buff_to_data(const_cast<char *>(store.c_str()), restOfData);
            dataInBuffer = true;
        }
    }
    
    return request;
}

void SocketConnection::send(std::string message) {
    if (::send(fd, message.c_str(), message.size(), 0) < 0)
        throw(SendFailureException());
}

char const* SocketConnection::CloseFailureException::what(void) const throw() {
    return ("webserv::SocketConnection failure in close()");
}

char const* SocketConnection::SendFailureException::what(void) const throw() {
    return ("webserv::SocketConnection failure in send()");
}

char const* SocketConnection::RecvFailureException::what(void) const throw() {
    return ("webserv::SocketConnection failure in recv()");
}
} // namespace webserv
