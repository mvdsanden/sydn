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
  syd::net::address remoteAddress(syd::net::ipv4, "10.1.1.10", 80);
  syd::net::connected_socket socket(syd::net::stream, remoteAddress);
  
  socket.write("GET / HTTP/1.1\nHOST: www.test.com\n\n");

  if (!socket) {
    std::cerr << "could not connect or write request" << std::endl;
    return 2;
  }

  do {
    char buffer[1024];
    std::streamsize count = socket.readSome(buffer, sizeof(buffer));
    std::cout.write(buffer, count);
  } while (socket);

  return 0;
}

```

### Blocking streaming IPv4 client with DNS resolve

```
#include <iostream>
#include <net>

int main(int argc, char *argv[]) {
  syd::net::resolver resolver;
  
  std::vector<syd::net::address> addresses;
  addresses = resolver.lookup(syd::net::stream, "www.test.com", 80).get(); // future

  // Connect to the first succeeding address.
  syd::net::connected_socket socket(addresses.begin(), addresses.end());
  
  socket.write("GET / HTTP/1.1\nHOST: www.test.com\n\n");

  if (!socket) {
    std::cerr << "could not connect or write request" << std::endl;
    return 2;
  }

  do {
    char buffer[1024];
    std::streamsize count = socket.readSome(buffer, sizeof(buffer));
    std::cout.write(buffer, count);
  } while (socket);

  return 0;
}
```
