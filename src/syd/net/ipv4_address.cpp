// ipv4_address.cpp                                                   -*-c++-*-
#include <ipv4_address.h>

#include <arpa/inet.h>
#include <cstring>
#include <string>

namespace syd {
namespace net {

ipv4_address::ipv4_address(std::string const &address, size_t port)
{
  auto addr = reinterpret_cast<sockaddr_in*>(this);
  addr->sin_family = AF_INET;
  setPort(port);
  setAddress(address);
}

bool ipv4_address::setAddress(std::string const &address)
{
  auto addr = reinterpret_cast<sockaddr_in *>(this);
  return 1 == inet_pton(addr->sin_family, address.c_str(), &addr->sin_addr);
}

void ipv4_address::setPort(size_t port)
{
  auto addr      = reinterpret_cast<sockaddr_in *>(this);
  addr->sin_port = htons(port);
}

std::string ipv4_address::address() const
{
  auto addr = reinterpret_cast<sockaddr_in const *>(this);  
  char buf[INET_ADDRSTRLEN] = {0};
  return inet_ntop(addr->sin_family, &addr->sin_addr, buf, sizeof(buf));
}

size_t ipv4_address::port() const
{
  auto addr = reinterpret_cast<sockaddr_in const*>(this);
  return ntohs(addr->sin_port);
}

} // namespace net
} // namespace syd

