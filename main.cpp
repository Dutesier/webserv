// #include "SocketListener.hpp"
// #include "SocketConnection.hpp"
// #include "Client.hpp"
// #include "ConfigSetup.hpp"
//
//
// // Proof of concept for sending and receiving an HTTP request and response
// void client_hello_world() {
//     SocketListener server(AF_INET, 8089);
//     server.bind_to_address();
//     server.start_listening();
//     
//     Client client(AF_INET, 8089);
//
//     server.accept_connections();
//     client.send_message("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Lenght: 14\r\n\r\nHello, world!");
//     std::cout << server.read_from_connection(NULL) << std::endl;
// }
//
// // Proof of concept for sending and receiving an HTTP request and response
// void browser_hello_world() {
//     SocketListener server(AF_INET, 8089);
//     server.bind_to_address();
//     server.start_listening();
//     
//     server.accept_connections();
//     std::cout << server.read_from_connection(NULL) << std::endl;
//     server.write_to_connection(NULL, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Lenght: 14\r\n\r\nHello, world!");
// }
//
//
// int main(int argc, char* argv[]) {
//     ConfigSetup setup(argc, argv);
//     Config* config = setup.get_config();
//     LOG_D( config == NULL ? "Config is NULL" : "Config was created");
//     //client_hello_world();
//     browser_hello_world();
//     return 0;
// }

// #include <iostream>
// #include "ConfigParser.hpp"
// #include "Fail.hpp"

// int	main( int argc, char *argv[] ) {

// 	( void )argc;
// 	( void )argv;

// 	std::string		filename;
// 	Parser*	parser = new ConfigParser();

// 	if ( argc > 1 ) filename = std::string(argv[1]);
// 	else filename = "./webserv.tmp";

// 	parser->parse(filename);
// 	if ( parser->error() )
// 		std::cout << *parser->error() << std::endl;

// 	delete parser;

// 	return ( 0 );
// }

#include "ConfigSetup.hpp"

class server_example {
public:
	server_example(int argc, char* argv[]): my_conf(NULL) {
		ConfigSetup temp(argc, argv);

		my_conf = temp.get_config();
	}

	~server_example(){
		if (my_conf != NULL)
			delete my_conf;
	}
private:
	Config* my_conf;
};

int	main( int argc, char *argv[] ) {


	server_example example(argc, argv);

	return ( 0 );
}

