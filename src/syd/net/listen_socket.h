// listen_socket.h                                                    -*-c++-*-
#ifndef INCLUDED_SYD_NET_LISTEN_SOCKET
#define INCLUDED_SYD_NET_LISTEN_SOCKET

#include <address.h>
#include <connected_socket.h>

#include <gsl/span>

#include <cstddef>
#include <system_error>

namespace syd {
namespace net {

/**
 * A socked used for listen protocols, like stream sockets.
 */
class listen_socket
{
  // DATA
  mutable std::error_condition d_last_error;
  address                      d_local_address;
  int                          d_fd = -1;

  // PRIVATE MANIPULATORS
  bool check_result(int result_code) const;

public:
  // CREATORS

  /**
   * Create a socket with the specified 'type' that listes on the specified
   * 'local_address'.
   */
  listen_socket(type type, address const &local_address);

  ~listen_socket();

  // MANIPULATORS

  /**
   * Accept the next pending connection into the specified 'socket'.
   */
  listen_socket &accept(connected_socket &socket);

  /**
   * Accept the next pending connection into the specified 'socket' and set the
   * specified 'address' to the remote address of the connection.
   */
  listen_socket &accept(connected_socket &socket, address &address);

  // ACCESSORS

  /**
   * Return 'false' when an error has occured, 'true' otherwise.
   */
  operator bool() const;

  /**
   * Return 'false' when an error has occured, 'true' otherwise.
   */
  bool okay() const;

  /**
   * Return the socket local address.
   */
  address local_address() const;

  /**
   * Return 'true' if the previous 'write' or 'read' would have blocked.
   */
  bool would_have_blocked() const;

  /**
   * Return the current error condition.
   */
  std::error_condition const &error() const;
};

// ------------------------------ INLINE METHODS ------------------------------

inline listen_socket::operator bool() const { return okay(); }

inline bool listen_socket::okay() const {
  return !d_last_error && !would_have_blocked();
}

inline address listen_socket::local_address() const { return d_local_address; }
 
inline bool listen_socket::would_have_blocked() const
{
  return d_last_error == std::errc::operation_would_block ||
         d_last_error == std::errc::resource_unavailable_try_again;
}

inline std::error_condition const &listen_socket::error() const
{
  return d_last_error;
}

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_LISTEN_SOCKET
