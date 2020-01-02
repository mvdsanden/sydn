// address.h                                                          -*-c++-*-
#ifndef INCLUDED_SYD_NET_ADDRESS
#define INCLUDED_SYD_NET_ADDRESS

#include <mem_sdo.h>

#include <iosfwd>
#include <sys/socket.h>
#include <type_traits>

namespace syd {
namespace net {

enum class address_family : int
{
  Unspecified = AF_UNSPEC,
  IPv4        = AF_INET,
  IPv6        = AF_INET6,
  Unix        = AF_UNIX
};

inline bool operator==(const address_family &lhs, int rhs)
{
  return static_cast<int>(lhs) == rhs;
}

inline bool operator==(int lhs, const address_family &rhs)
{
  return lhs == static_cast<int>(rhs);
}

inline bool operator!=(const address_family &lhs, int rhs)
{
  return static_cast<int>(lhs) != rhs;
}

inline bool operator!=(int lhs, const address_family &rhs)
{
  return lhs != static_cast<int>(rhs);
}
  
class address
{
  static const size_t c_sdoSize = 32;
  mem::sdo<c_sdoSize> d_data;

public:
  /**
   * Allocate the specified 'size'.
   *
   * Behavior is undefined unless 'size' is zero or at least the size of
   * sockaddr.
   */
  void resize(size_t size);

  /**
   * Return the address family or Unspecified when size is zero.
   */
  int family() const;

  /**
   * Return the size of the address buffer.
   */
  size_t size() const { return d_data.size(); }

  /**
   * Return the capacity of the address buffer.
   */
  size_t capacity() const { return d_data.capacity(); }

  /**
   * Return a pointer to the native address buffer.
   */
  sockaddr const *native() const
  {
    return d_data.size() != 0
               ? reinterpret_cast<sockaddr const *>(d_data.data())
               : nullptr;
  }

  /**
   * Return a pointer to the native address buffer.
   */
  sockaddr *native()
  {
    return d_data.size() != 0 ? reinterpret_cast<sockaddr *>(d_data.data())
                              : nullptr;
  }

  bool operator==(std::nullptr_t) const
  {
    return family() != address_family::Unspecified;
  }

  bool operator!=(std::nullptr_t) const
  {
    return family() != address_family::Unspecified;
  }
};

std::ostream &operator<<(std::ostream &stream, address const &value);

//   template <typename T, typename S>
//   inline T address_cast(S value)
//   {
//     static_assert(std::is_same<address, typename
//     std::remove_pointer<S>::type>::value, "S is not an address");
//     static_assert(std::is_base_of<address, typename
//     std::remove_pointer<T>::type>::value, "address is not convertable to
//     return type"); return *reinterpret_cast<T*>(&value);
// }
  
} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_ADDRESS
