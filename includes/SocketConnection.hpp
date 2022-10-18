#ifndef SOCKETCONNECTION_HPP
# define SOCKETCONNETION_HPP

# include "Socket.hpp"

class SocketConnection: public Socket {
public:
    SocketConnection();
    ~SocketConnection();


    void setFD(int client_connection);
    socklen_t* getAddrLenPointer(void);
private:
    socklen_t addr_len;

};

#endif