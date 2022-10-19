#ifndef SOCKETCONNECTION_HPP
# define SOCKETCONNETION_HPP

# include "Socket.hpp"
# ifndef READING_BUFFER
#  define READING_BUFFER 1024
# endif

class SocketConnection: public Socket {
public:
    SocketConnection();
    ~SocketConnection();


    void setFD(int client_connection);
    void setAddress(struct sockaddr* addr);
    socklen_t* getAddrLenPointer(void);

    std::string read_connection(void);
    bool        write_connection(std::string message);

private:
    socklen_t addr_len;

};

#endif

// std::string SocketConnection::read_connection(void) {
// 	char buff[1024 + 1]; // This will probably not be a fized size
// 	size_t bytes_read;
// 	std::string temp;

// 	if ((bytes_read = recv(fd, &buff, 1024, MSG_DONTWAIT)) < 0)
// 		return "No message in connection - RECV failed";
// 	else
// 		buff[bytes_read] = '\0';
// 	return temp;
// }