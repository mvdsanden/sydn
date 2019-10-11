// address.cpp                                                        -*-c++-*-
#include <address.h>

#include <iostream>

namespace syd {
namespace net {

namespace internal {

  template <int Family>
  void printAddress(std::ostream *stream, sockaddr_storage const *address)
  {
    *stream << "Unknown address family: " << static_cast<int>(address->ss_family);
  }

} // namespace internal

  int address::family() const
  {
    return d_storage.ss_family;
  }


  std::ostream &operator<<(std::ostream &stream, address const &address)
  {
    auto addr = reinterpret_cast<sockaddr_storage const *>(&address);
    switch (address.family()) {
    case AF_INET:
      internal::printAddress<AF_INET>(&stream, addr);
      break;

    case AF_INET6:
      internal::printAddress<AF_INET6>(&stream, addr);
      break;

    case AF_UNIX:
      internal::printAddress<AF_UNIX>(&stream, addr);
      break;

    default:
      internal::printAddress<-1>(&stream, addr);
      break;
    };

    return stream;
  }

} // namespace net
} // namespace syd
