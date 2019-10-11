// address.h                                                          -*-c++-*-
#ifndef INCLUDED_SYD_NET_ADDRESS
#define INCLUDED_SYD_NET_ADDRESS

#include <iosfwd>
#include <sys/socket.h>
#include <type_traits>

namespace syd {
namespace net {

  enum class address_family {
    IPv4 = AF_INET,
    IPv6 = AF_INET6,
    Unix = AF_UNIX
  };
  
class address
{
  sockaddr_storage d_storage = {0};
public:
  /**
   * Return the address family.
   */
  int family() const;
};
  
  std::ostream &operator<<(std::ostream &stream, address const &value);

  template <typename T, typename S>
  inline T address_cast(S value)
  {
    static_assert(std::is_same<address, typename std::remove_pointer<S>::type>::value, "S is not an address");
    static_assert(std::is_base_of<address, typename std::remove_pointer<T>::type>::value, "address is not convertable to return type");
    return *reinterpret_cast<T*>(&value);
}

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_ADDRESS
