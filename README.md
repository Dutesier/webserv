# Webserv

This project is about writing an HTTP server and to be able to test it with an
actual browser.
HTTP is one of the most used protocols on the internet.
Knowing its arcane will be useful, even if you won’t be working on a website.

## Installation

```shell
git clone https://github.com/Dutesier/webserv
cd webserv
```

## Compilation

```shell
# In webser root dir
mkdir build/
cd build/
cmake -D test=ON ..
make
./webserv
## Running Unit Tests
./tests/webserv_test
```

<!-- ```mermaid
classDiagram

class ASocket
ASocket   :   getFD()
ASocket   :   getAddressPointer()
ASocket   :   struct sockaddr address
ASocket   :   int fd

class SocketListener
SocketListener  :   bind()
SocketListener  :   listen()
SocketListener  :   accept()
SocketListener  :   struct sockaddr address
SocketListener  :   int fd
SocketListener  :   SocketConnection[] connections


class SocketConnection
SocketConnection    :   struct sockaddr address
SocketConnection    :   int fd
SocketConnection   :   write()
SocketConnection   :   read()

class Server
Server  :   SocketListener[] listeners

ASocket <|-- SocketListener
ASocket <|-- SocketConnection
Server *--ASocket
``` -->
