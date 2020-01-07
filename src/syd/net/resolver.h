// resolver.h                                                         -*-c++-*-
#ifndef INCLUDED_SYD_NET_RESOLVER
#define INCLUDED_SYD_NET_RESOLVER

#include <socket.h>
#include <address_info_list.h>

namespace syd {
namespace net {

/**
 * 
 */
class resolver
{
public:
  // CREATORS

  /**
   * Create an uninitialized (and not yet connected) socket.
   */
  resolver();

  // MANIPULATORS

  std::future<address_info_list> lookup(family             family,
                                        type               type,
                                        std::string const &node,
                                        std::string const &service);

};

// ------------------------------ INLINE METHODS ------------------------------

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_RESOLVER
