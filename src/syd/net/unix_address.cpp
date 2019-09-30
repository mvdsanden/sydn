// unix_address.cpp                                                   -*-c++-*-
#include <unix_address.h>

#include <sys/un.h>

#include <string>
#include <cassert>

namespace syd {
namespace net {

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
    memcpy(addr->sun_path, path.c_str(), path.length());
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
