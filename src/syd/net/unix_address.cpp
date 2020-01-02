// unix_address.cpp                                                   -*-c++-*-
#include <address.ih>
#include <unix_address.h>

#include <sys/un.h>

#include <cassert>
#include <iostream>
#include <string>

namespace syd {
namespace net {

namespace internal {
template <>
void printAddress<AF_UNIX>(std::ostream *  stream,
                           sockaddr const *address,
                           size_t          length)
{
  auto const *addr = reinterpret_cast<sockaddr_un const *>(address);
  *stream << "unix:" << addr->sun_path;
}
} // namespace internal

unix_address::unix_address(std::string const &path)
{
  resize(sizeof(sockaddr_un));
  
  auto addr        = reinterpret_cast<sockaddr_un *>(native());
  addr->sun_family = AF_UNIX;
  set_path(path);
}

bool unix_address::set_path(std::string const &path)
{
  auto addr = reinterpret_cast<sockaddr_un *>(native());
  if (sizeof(addr->sun_path) < path.length()) {
    return false;
  }
  strncpy(addr->sun_path, path.c_str(), sizeof(addr->sun_path));
  return true;
}

std::string unix_address::path() const
{
  assert(sizeof(sockaddr_un) <= size());
  auto addr = reinterpret_cast<sockaddr_un const *>(native());
  return addr->sun_path;
}

} // namespace net
} // namespace syd
