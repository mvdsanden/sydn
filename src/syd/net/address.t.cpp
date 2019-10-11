// address.t.cpp                                                      -*-c++-*-
#include <iostream>

#include <address.h>
#include <ipv4_address.h>

int main(int argc, char *argv[]) {
  syd::net::address addr;

  std::cout << addr << std::endl;

  addr = syd::net::ipv4_address("10.0.0.1", 80);

  std::cout << addr << std::endl;

  std::string test;
  
  auto ipv4Address = syd::net::address_cast<syd::net::ipv4_address>(addr);

  std::cout << ipv4Address << std::endl;
  
  return 0;
}
