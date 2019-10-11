// address.t.cpp                                                      -*-c++-*-
#include <iostream>

#include <address.h>
#include <ipv4_address.h>

int main(int argc, char *argv[]) {
  syd::net::address addr;

  std::cout << addr << std::endl;

  addr = syd::net::ipv4_address("10.0.0.1", 80);

  std::cout << addr << std::endl;

  auto ipv4Address = syd::net::address_cast<syd::net::ipv4_address>(addr);
  auto ipv4Address1 = syd::net::address_cast<syd::net::ipv4_address*>(&addr);
  auto ipv4Address2 = syd::net::address_cast<syd::net::ipv4_address>(syd::net::address());

  //  auto ipv4Address2 = syd::net::address_cast<syd::net::ipv4_address*>(&addr);

  std::cout << ipv4Address << std::endl;
  std::cout << *ipv4Address1 << std::endl;
  std::cout << ipv4Address2 << std::endl;
  
  return 0;
}
