// connected_socket.h                                                 -*-c++-*-
#ifndef INCLUDED_SYD_NET_CONNECTED_SOCKET
#define INCLUDED_SYD_NET_CONNECTED_SOCKET

#include <address.h>
#include <socket.h>

#include <gsl/span>

#include <cstddef>
#include <system_error>

namespace syd {
namespace net {

/**
 * A socked used for connected protocols, like stream sockets.
 */
class connected_socket
{
  friend class listen_socket;
  
  mutable std::error_condition d_last_error;
  size_t                       d_last_bytes = 0;
  int                          d_fd         = -1;
  mutable bool                 d_okay : 1;
  bool                         d_would_have_blocked : 1;

  bool check_result(int result_code) const;
  void initialize_from_fd(int fd);
  
public:
  // CREATORS

  /**
   * Create an uninitialized (and not yet connected) socket.
   */
  connected_socket();
  
  /**
   * Create a socket with the specified 'type' connected to the specified
   * 'remote_address' and optionally bound to the specified 'local_address'.
   */
  connected_socket(type           type,
                   address const &remote_address,
                   address const &local_address = address());

  connected_socket(connected_socket const &) = delete;
  connected_socket(connected_socket &&) = delete;
  
  ~connected_socket();
  
  // MANIPULATORS

  connected_socket &operator=(connected_socket const &) = delete;
  connected_socket &operator=(connected_socket &&) = delete;

  /**
   * Write the specified 'data' to the socket. See 'last_bytes' for the
   * number of bytes written. If an error occurs the socket will compare to
   * 'false' after this call and 'error' will return an appropriate error
   * condition.
   */
  connected_socket &write(gsl::span<const char> data);
  
  /**
   * Read the specified 'data' from the socket. See 'read_count' for the number
   * of bytes read. If an error occurs the socket will compare to 'false' after
   * this call and 'error' will return an appropriate error condition.
   */
  connected_socket &read(gsl::span<char> data);

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
   * Return the last number of bytes wittten or read.
   */
  size_t last_bytes() const;

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

inline connected_socket::operator bool() const { return d_okay; }

inline bool connected_socket::okay() const { return d_okay; }

inline size_t connected_socket::last_bytes() const { return d_last_bytes; }

inline bool connected_socket::would_have_blocked() const
{
  return d_would_have_blocked;
}

inline std::error_condition const &connected_socket::error() const
{
  return d_last_error;
}

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_CONNECTED_SOCKET
