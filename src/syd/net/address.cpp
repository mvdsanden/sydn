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

void address::printTo(std::ostream *stream) const
{
  switch (d_storage.ss_family) {
  case AF_INET: internal::printAddress<AF_INET>(stream, &d_storage); return;
  case AF_INET6: internal::printAddress<AF_INET6>(stream, &d_storage); return;
  case AF_UNIX: internal::printAddress<AF_UNIX>(stream, &d_storage); return;
  default: internal::printAddress<-1>(stream, &d_storage); return;
  };
}

} // namespace net
} // namespace syd
