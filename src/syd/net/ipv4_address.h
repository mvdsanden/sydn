// ipv4_address.h                                                     -*-c++-*-
#ifndef INCLUDED_SYD_NET_IPV4_ADDRESS
#define INCLUDED_SYD_NET_IPV4_ADDRESS

#include <address.h>

namespace syd {
namespace net {

class ipv4_address : public address
{
public:
  ipv4_address(std::string const &address, size_t port);

  bool setAddress(std::string const &address);

  void setPort(size_t port);
  
  std::string address() const;

  size_t port() const;
};

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_ADDRESS
