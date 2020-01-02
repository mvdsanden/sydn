// ipv4_address.h                                                     -*-c++-*-
#ifndef INCLUDED_SYD_NET_IPV4_ADDRESS
#define INCLUDED_SYD_NET_IPV4_ADDRESS

#include <address.h>

namespace syd {
namespace net {

class ipv4_address : public address
{
public:
  /**
   * Creates ipv4 address that points to "0.0.0.0:0".
   */
  ipv4_address();
  
  /**
   * Create ipv4 address with the specified 'address' and the specified 'port'.
   * If 'address' is not a valid address it will default to "0.0.0.0".
   */
  ipv4_address(std::string const &address, size_t port);

  /**
   * Set the specified 'address'. If 'address' is valid return 'true', otherwise
   * return 'false'.
   */
  bool set_address(std::string const &address);

  /**
   * Set the specified 'port' number.
   */
  void set_port(size_t port);

  /**
   * Return the address string.
   */
  std::string address() const;

  /**
   * Return the port number.
   */
  size_t port() const;
};

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_ADDRESS
