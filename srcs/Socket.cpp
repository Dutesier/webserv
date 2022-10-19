#include "Socket.hpp"

Socket::Socket(): fd(-1), address(NULL){

}

Socket::~Socket(){

}

/* ************************************************************************** */
/* Getters and Setters                                                        */
/* ************************************************************************** */

int Socket::get_fd(void) const {
    return (this->fd);
}

struct sockaddr* Socket::get_address(void) const {
    return (this->address);
}