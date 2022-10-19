#include "Client.hpp"

Client::Client(int domain, in_port_t port){
    connection.set_fd(socket(AF_INET, SOCK_STREAM, 0));
    if (connection.get_fd() == -1)
        perror("Client: Socket");

    struct sockaddr_in* temp = new struct sockaddr_in;

    temp->sin_family = AF_INET;
    temp->sin_port = htons(port);
    connection.set_address(reinterpret_cast<struct sockaddr *>(temp));

    if (inet_pton(AF_INET, "127.0.0.1", &temp->sin_addr) <= 0)
        perror("Client: Inet_pton");
    
    if (connect(connection.get_fd(), connection.get_address(), sizeof(*connection.get_address())) < 0)
        perror("Client: Connect");
}

Client::~Client(){

}

void    Client::send_message(std::string message) {
    connection.write_connection(message);
}

std::string Client::receive_message(void) {
    return (connection.read_connection());
}