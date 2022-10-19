#ifndef SOCKETCONNECTION_HPP
# define SOCKETCONNECTION_HPP

# include "Socket.hpp"
# ifndef READING_BUFFER
#  define READING_BUFFER 1024
# endif

class SocketConnection: public Socket {
public:
    SocketConnection();
    ~SocketConnection();


    void set_fd(int client_connection);
    void set_address(struct sockaddr* addr);
    socklen_t* get_addr_len_ptr(void);

    std::string read_connection(void);
    bool        write_connection(std::string message);

private:
    socklen_t addr_len;

};

#endif
