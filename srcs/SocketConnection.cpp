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
        header.insert(eoh_position + 4, "");
    }
    request = parser.parse_header(header);
    // If there is more data to be dealt with
    if (strlen(buff) > eoh_position + 4) {
        // If we have a Content-Lenght
        if (request->headers.find("Content-Lenght") != request->headers.end()) {
            int body_size = atoi(request->headers.find("Content-Lenght")->second.c_str());
            if (body_size < MAX_BODY_SIZE) {
                request->content.reserve(body_size);
                for (int i = 0, j = eoh_position + 4; i < body_size; i++) {

                    if (j == READING_BUFFER) {
                        // We need to keep making recv call until we get something (or timeout which we would implement here)
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
                        j = 0; // Resetting j to 0 because it is the position in the newly aquired buff
                    }

                    // Adding character by character
                    request->content.push_back(buff[j]);
                    j++;
                }
            }
        } else {
            // Right now if no Content-Lenght is provided we're jumping into the next RequestLine and ignoring the content
            std::string before_eoh(buff);
            std::string after_eoh = before_eoh.substr(eoh_position);

            // if we can find a new RequestLine we'll store from that point to restOfData
            // otherwise we'll just clear the buffer
            int next = parser.find_next_request(after_eoh.c_str());
            if (next == -1) {
                dataInBuffer = false;
            } else {
                std::string temp = after_eoh.substr(next);
                buff_to_data(const_cast<char *>(temp.c_str()), restOfData);
                dataInBuffer = true;
            }
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
