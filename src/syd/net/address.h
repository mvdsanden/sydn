// address.h                                                          -*-c++-*-
#ifndef INCLUDED_SYD_NET_ADDRESS
#define INCLUDED_SYD_NET_ADDRESS

#include <iosfwd>
#include <sys/socket.h>

namespace syd {
namespace net {

class address
{
  sockaddr_storage d_storage = {0};
public:
  /**
   * Return the address family.
   */
  int family() const;

  /**
   * Print the address to the specified 'stream'.
   */
  void printTo(std::ostream *stream) const;
};

inline std::ostream &operator<<(std::ostream &stream, address const &value)
{
  value.printTo(&stream);
  return stream;
}

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_ADDRESS
