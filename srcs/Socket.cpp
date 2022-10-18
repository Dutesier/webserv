#include "Socket.hpp"

Socket::Socket(): fd(-1), address(NULL){

}

Socket::~Socket(){

}

/* ************************************************************************** */
/* Getters and Setters                                                        */
/* ************************************************************************** */

int Socket::getFD(void) const {
    return (this->fd);
}

struct sockaddr* Socket::getAddress(void) const {
    return (this->address);
}