// echoserver.cpp                                                     -*-c++-*-

#include <listen_socket.h>
#include <ipv4_address.h>

#include <iostream>
#include <vector>
#include <future>

class EchoServer
{
  // TYPES
  struct ClientSession
  {
    // PUBLIC DATA
    syd::net::connected_socket d_socket;
    syd::net::address          d_address;

    // MANIPULATORS
    void run()
    {
      std::cout << "Started session with: " << d_address << ".\n";
      char buffer[128];
      while (d_socket.read(buffer) &&
             0 != d_socket.last_bytes()) {
        d_socket.write(
            gsl::make_span(buffer, d_socket.last_bytes()));
      }
      std::cout << "Ended session with: " << d_address << ".\n";
    }

    // void run()
    // {
    //   std::cout << "Started session with: " << d_address << ".\n";

    //   auto read = syd::make_connected_reader(d_socket);
    //   auto write = syd::make_connected_writer(d_socket);
      
    //   char buffer[128];

    //   while (read(buffer) && 0 != read.last_bytes()) {
    //     write(gsl::make_span(buffer, read.last_bytes()));
    //   }

    //   // TODO:
    //   // while (syd::net::copy(read, write, 128));
      
    //   std::cout << "Ended session with: " << d_address << ".\n";
    // }


  };

  // DATA
  syd::net::ipv4_address d_serverAddress;

public:
  // CREATORS
  EchoServer()
      : d_serverAddress("127.0.0.1", 0)
  {}

  // MANIPULATORS
  int run()
  {
    syd::net::listen_socket listenSocket(syd::net::type::Stream,
                                         d_serverAddress);

    //    auto accepter = syd::net::make_accepter(listenSocket);
    
    if (!listenSocket) {
      std::cerr << listenSocket.error().message();
      return 1;
    }

    std::cout << "Echoserver running on: " << listenSocket.local_address()
              << ".\n";

    while (true) {      
      auto pendingSession = std::make_unique<ClientSession>();

      listenSocket.accept(pendingSession->d_socket, pendingSession->d_address);

      std::thread(
          [session{std::move(pendingSession)}]() mutable { session->run(); })
          .detach();
    }
  }
};

// GLOBAL METDHODS
int main(int argc, char **argv)
{
  std::cout << "Size: " << sizeof(syd::net::connected_socket) << ".\n";

  EchoServer server;
  return server.run();
}
