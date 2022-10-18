#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <unistd.h> // Let's get back to this, can we use close()?

class Socket {
public:
    /* Constructors and Destructors */
    Socket();
    virtual ~Socket();

    /* Getters and Setters */
    int getFD(void) const;
    struct sockaddr* getAddress(void) const;

protected:
    /* Protected Attributes */
    int                 fd;
    struct sockaddr*    address;
};

#endif