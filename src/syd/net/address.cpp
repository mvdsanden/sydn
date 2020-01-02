// address.cpp                                                        -*-c++-*-
#include <address.h>

#include <cassert>
#include <cstring>
#include <iostream>

namespace syd {
namespace net {

namespace internal {

template <int Family>
void printAddress(std::ostream *stream, sockaddr const *address, size_t length)
{
  *stream << "Unknown address family: " << static_cast<int>(address->sa_family);
}

} // namespace internal

void address::resize(size_t size) { d_data.resize(); }

int address::family() const { return native()->sa_family; }

std::ostream &operator<<(std::ostream &stream, address const &address)
{
  switch (address.family()) {
  case AF_INET:
    internal::printAddress<AF_INET>(&stream, address.native(), address.size());
    break;

  case AF_INET6:
    internal::printAddress<AF_INET6>(&stream, address.native(), address.size());
    break;

  case AF_UNIX:
    internal::printAddress<AF_UNIX>(&stream, address.native(), address.size());
    break;

  default:
    internal::printAddress<-1>(&stream, address.native(), address.size());
    break;
  };

  return stream;
}

} // namespace net
} // namespace syd
