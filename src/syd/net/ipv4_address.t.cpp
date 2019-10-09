// ipv4_address.cpp                                                   -*-c++-*-
#include <iostream>
#include <ipv4_address.h>
#include <cassert>

int main(int argc, char *argv[]) {

  syd::net::ipv4_address addr("10.0.0.1", 10);


  assert(AF_INET == addr.family());
  assert("10.0.0.1" == addr.address());
  assert(10 == addr.port());
  
  std::cout << addr << std::endl;

  addr.setAddress("127.0.0.1");
  addr.setPort(80);

  assert("127.0.0.1" == addr.address());
  assert(80 == addr.port());

  assert(!addr.setAddress("test"));

  std::cout << addr << std::endl;
  
  return 0;
}
