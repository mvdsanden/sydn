// connected_socket.h                                                 -*-c++-*-
#ifndef INCLUDED_SYD_NET_CONNECTED_SOCKET
#define INCLUDED_SYD_NET_CONNECTED_SOCKET

#include <address.h>
#include <socket.h>

#include <gsl/span>

#include <cassert>
#include <cstddef>
#include <system_error>

namespace syd {
namespace net {

/**
 * A socked used for connected protocols, like stream sockets.
 */
class connected_socket
{
  int d_fd = -1;

public:
  // CREATORS

  /**
   * Create an uninitialized (and not yet connected) socket.
   */
  connected_socket();  
  ~connected_socket();
  
  // MANIPULATORS

  /**
   * Connect with the specified 'type' to the specified
   * 'remote_address' and optionally bound to the specified 'local_address'.
   */
  std::error_condition connect(type           type,
                               address const &remote_address,
                               address const &local_address = address());

  /**
   * Import the specified 'fileDescriptor'.
   */
  void import(int fileDescriptor);
  
  /**
   * Write the specified 'data' to the socket. See 'last_bytes' for the
   * number of bytes written. If an error occurs the socket will compare to
   * 'false' after this call and 'error' will return an appropriate error
   * condition.
   */
  std::pair<std::error_condition, size_t> write(gsl::span<const char> data);
  
  /**
   * Read the specified 'data' from the socket. See 'read_count' for the number
   * of bytes read. If an error occurs the socket will compare to 'false' after
   * this call and 'error' will return an appropriate error condition.
   */
  std::pair<std::error_condition, size_t> read(gsl::span<char> data);

  // ACCESSORS

  /**
   * Return the socket local address.
   */
  address local_address() const;

};

// ------------------------------ INLINE METHODS ------------------------------

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_CONNECTED_SOCKET
