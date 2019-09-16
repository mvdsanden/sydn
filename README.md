# sydn
C++ Network Communication Classes


## Introduction

This is a case study to find the lean and meanest design for C++ socket classes, while supporting
the most general use cases.

## Current thoughts

These are some usage examples I think are simple yet powerfull.

### Blocking streaming IPv4 client

``` 
#include <iostream>
#include <net>

int main(int argc, char *argv[]) {
  syd::net::address remoteAddress = syd::net::make_ipv4_address("10.1.1.10", 80);
  syd::net::connected_socket socket(syd::net::type::stream, remoteAddress);
  
  socket.write("GET / HTTP/1.1\nHOST: www.test.com\n\n");

  if (!socket) {
    std::cerr << "could not connect or write request: " << socket.error().message() << std::endl;
    return 2;
  }

  socket.shutdown(syd::net::writing);

  do {
    char buffer[1024];
    std::streamsize count = socket.readSome(buffer, sizeof(buffer));
    std::cout.write(buffer, count);
  } while (socket);

  return 0;
}

```

### Blocking streaming any protocol client with DNS resolve

```
#include <iostream>
#include <net>

int main(int argc, char *argv[]) {
  syd::net::resolver resolver;
  
  syd::net:address_info_list addresses;
  addresses = resolver.lookup(syd::net::family::any, syd::net::type::stream, "www.test.com", "80").get(); // future

  // Connect to the first succeeding address.
  syd::net::connected_socket socket(addresses.begin(), addresses.end());
  
  socket.write("GET / HTTP/1.1\nHOST: www.test.com\n\n");

  if (!socket) {
    std::cerr << "could not connect or write request: " << socket.error().message() << std::endl;
    return 2;
  }

  socket.shutdown(syd::net::shutdown::writing);

  do {
    char buffer[1024];
    std::streamsize count = socket.readSome(buffer, sizeof(buffer));
    std::cout.write(buffer, count);
  } while (socket);

  return 0;
}
```

### Blocking streaming IPv4 server

``` 
#include <iostream>
#include <net>

int main(int argc, char *argv[]) {
  syd::net::address localAddress = syd::net::make_ipv4_address("0.0.0.0", 80);
  syd::net::listen_socket listenSocket(syd::net::type::stream, remoteAddress);
  
  if (!listenSocket) {
    std::cerr << "Could not create listen socket: " << listenSocket.error().message() << std::endl;
    return 1;
  }
  
  while (true) {
    syd::net::address remoteAddress;
    syd::net::connected_socket remoteSocket;
  
    std::tie(remoteAddress, remoteSocket) = socket.accept();

    if (!listenSocket) {
      std::cerr << "could not accept connection: " << listenSocket.error().message() << std::endl;
      return 2;
    }

    do {
      char buffer[1024];
      std::streamsize count = socket.readSome(buffer, sizeof(buffer));
      std::cout.write(buffer, count);
    } while (0 != count);  

    remoteSocket.write("HTTP/1.1 501 Not Implemented\r\n\r\n");
  }

  return 0;
}

```

### Blocking datagram IPv4 client

``` 
#include <iostream>
#include <net>

struct Packet {
  uint16_t d_version;
  uint16_t d_data0;
  uint32_t d_data1;  
};

int main(int argc, char *argv[]) {

  syd::net::unconnected_socket socket(syd::net::type::datagram);

  Packet packet{1,2,3};

  syd::net::address sendAddress = syd::net::make_ipv4_address("10.1.1.10", 3000);
  socket.write(packet, sendAddress);

  if (!socket) {
    std::cerr << "could not write packet: " << socket.error().message() << std::endl;
    return 2;
  }

  syd::net::address receiveAddress;
  
  while (true) {
  
    std::tie(receiveAddress, packet) = socket.receive<Packet>();
    
    if (!socket) {
      std::cerr << "Error receiving packet: << socket.error().message() << std:endl;
      return 3;
    }
  
  }

  return 0;
}

```


# Random thoughts

* For non-blocking add `bool socket::would_block() const`?

