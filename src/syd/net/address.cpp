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

template <>
void printAddress<AF_UNSPEC>(std::ostream *  stream,
                             sockaddr const *address,
                             size_t          length)
{
  *stream << "Unspecified address";
}
} // namespace internal

void address::resize(size_t size)
{
  assert(size == 0 || size >= sizeof(sockaddr));
  d_data.resize(size);
}

int address::family() const
{
  return size() != 0 ? native()->sa_family : AF_UNSPEC;
}

std::ostream &operator<<(std::ostream &stream, address const &address)
{
#define CASE(x)                                                                \
  case x:                                                                      \
    internal::printAddress<x>(&stream, address.native(), address.size());      \
    break

  switch (address.family()) {
    CASE(AF_UNSPEC);
    CASE(AF_INET);
    CASE(AF_INET6);
    CASE(AF_UNIX);

  default:
    internal::printAddress<-1>(&stream, address.native(), address.size());
    break;
  };

  return stream;
#undef CASE
}

} // namespace net
} // namespace syd
