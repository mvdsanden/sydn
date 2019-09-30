// address.cpp                                                        -*-c++-*-
#include <address.h>

#include <iostream>

#include <netdb.h>

namespace syd {
namespace net {

void address::printTo(std::ostream *stream) const
{

  char host[NI_MAXHOST];
  char port[NI_MAXSERV];

  int res = getnameinfo(reinterpret_cast<sockaddr const *>(&d_storage),
                        sizeof(d_storage),
                        host,
                        sizeof(host),
                        port,
                        sizeof(port),
                        NI_NUMERICHOST);

  if (0 != res) {
    *stream << "invalid-address";
    return;
  }

  *stream << host << ":" << port;
}

} // namespace net
} // namespace syd
