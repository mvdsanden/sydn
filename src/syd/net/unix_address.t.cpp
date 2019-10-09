// unix_address.cpp                                                   -*-c++-*-
#include <iostream>
#include <unix_address.h>
#include <cassert>

int main(int argc, char *argv[]) {

  syd::net::unix_address addr("/tmp/test");

  assert(AF_UNIX == addr.family());
  assert("/tmp/test" == addr.path());
  
  std::cout << addr << std::endl;

  addr.setPath("hello");

  std::cout << "Path: " << addr.path() << std::endl;

  assert("hello" == addr.path());

  assert(!addr.setPath(std::string(110,'a')));

  std::cout << addr << std::endl;
  
  return 0;
}
