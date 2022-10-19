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