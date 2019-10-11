// ipv4_address.cpp                                                   -*-c++-*-
#include <ipv4_address.h>
#include <address.ih>

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>

namespace syd {
namespace net {

  namespace internal {
    template <>
    void printAddress<AF_INET>(std::ostream *stream,
			       sockaddr_storage const *address)
    {
      char host[NI_MAXHOST];
      char port[NI_MAXSERV];

      auto tmp = reinterpret_cast<sockaddr const *>(address);

      int res = getnameinfo(reinterpret_cast<sockaddr const *>(address),
			    sizeof(*address),
			    host,
			    sizeof(host),
			    port,
			    sizeof(port),
			    NI_NUMERICHOST
			    | NI_NUMERICSERV);

      if (0 != res) {
	*stream << "invalid-address(" << gai_strerror(res) << ")";
	return;
      }

      *stream << host << ":" << port;
    }

    template <>
    void printAddress<AF_INET6>(std::ostream *stream, sockaddr_storage const *address)
    {
      printAddress<AF_INET>(stream, address);
    }

  } // namespace internal
  

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

  std::string tmp;
  tmp.resize(INET_ADDRSTRLEN);
  inet_ntop(addr->sin_family, &addr->sin_addr, &tmp[0], INET_ADDRSTRLEN);
  tmp.resize(strnlen(tmp.data(), INET_ADDRSTRLEN));

  return tmp;
}

size_t ipv4_address::port() const
{
  auto addr = reinterpret_cast<sockaddr_in const*>(this);
  return ntohs(addr->sin_port);
}

} // namespace net
} // namespace syd

