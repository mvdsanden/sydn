// unix_address.cpp                                                   -*-c++-*-
#include <unix_address.h>
#include <address.ih>

#include <sys/un.h>

#include <cassert>
#include <iostream>
#include <string>

namespace syd {
namespace net {

  namespace internal {
    template <>
    void printAddress<AF_UNIX>(std::ostream *stream, sockaddr_storage const *address)
    {
      auto const *addr = reinterpret_cast<sockaddr_un const*>(address);
      *stream << "unix:" << addr->sun_path;
    }
  } // namespace internal

  unix_address::unix_address(std::string const &path)
  {
    auto addr = reinterpret_cast<sockaddr_un*>(this);
    addr->sun_family = AF_UNIX;
    setPath(path);
  }

  bool unix_address::setPath(std::string const &path)
  {
    auto addr = reinterpret_cast<sockaddr_un*>(this);
    if (sizeof(addr->sun_path) < path.length()) {
      return false;
    }
    strncpy(addr->sun_path, path.c_str(), sizeof(addr->sun_path));
    return true;
  }

  std::string unix_address::path() const
  {
    assert(sizeof(sockaddr_un) < sizeof(*this));
    auto addr = reinterpret_cast<sockaddr_un const*>(this);
    return addr->sun_path;
  }

} // namespace net
} // namespace syd
