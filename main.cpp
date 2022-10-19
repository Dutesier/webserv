#include "Socket.hpp"
#include "SocketListener.hpp"
#include <unistd.h>
#include <iostream>


// Proof of concept for sending and receiving an HTTP request and response
void hello_world() {
    SocketListener socket(AF_INET, 8089);

    if (!socket.bind_to_address())
        return static_cast<void>(std::cout << "bind" << std::endl);
    if (!socket.start_listening())
        return static_cast<void>(std::cout << "listen" << std::endl);
    struct sockaddr_in client;
    socklen_t sizeOfClient;
    
    int client_fd = accept(socket.get_fd(),
            reinterpret_cast<sockaddr *>(&client),
            &sizeOfClient);
    if (client_fd < 0)
        return static_cast<void>(std::cout << "accept" << std::endl);
    
    char *buffer = new char[300000];
    read(client_fd, buffer, 300000);
    write(client_fd, "HTTP/1.1 200 OK\n\
    Content-Type: text/plain\n\
    Content-Lenght: 14\n\n\
    Hello, world!", 87);
    close(client_fd);
    std::cout << buffer << std::endl;
    delete buffer;
    close(socket.get_fd());
}

int main() {
    hello_world();
    return 0;
}
