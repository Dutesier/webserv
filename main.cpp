#include "SocketListener.hpp"
#include "Client.hpp"


// Proof of concept for sending and receiving an HTTP request and response
void client_hello_world() {
    SocketListener server(AF_INET, 8089);
    server.bind_to_address();
    server.start_listening();
    
    Client client(AF_INET, 8089);

    server.accept_connections();
    client.send_message("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Lenght: 14\r\n\r\nHello, world!");
    std::cout << server.read_from_connection(NULL) << std::endl;
}

// Proof of concept for sending and receiving an HTTP request and response
void browser_hello_world() {
    SocketListener server(AF_INET, 8089);
    server.bind_to_address();
    server.start_listening();
    
    server.accept_connections();
    std::cout << server.read_from_connection(NULL) << std::endl;
    server.write_to_connection(NULL, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Lenght: 14\r\n\r\nHello, world!");
}

int main() {
    //client_hello_world();
    browser_hello_world();
    return 0;
}
