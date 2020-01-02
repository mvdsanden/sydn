// unix_address.h                                                     -*-c++-*-
#ifndef INCLUDED_SYD_NET_UNIX_ADDRESS
#define INCLUDED_SYD_NET_UNIX_ADDRESS

#include <address.h>

namespace syd {
namespace net {

class unix_address : public address
{
public:
  unix_address(std::string const &path);

  bool set_path(std::string const &path);

  std::string path() const;
};

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_ADDRESS
