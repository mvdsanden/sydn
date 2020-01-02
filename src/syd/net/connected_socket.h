// connected_socket.h -*-c++-*-
#ifndef INCLUDED_SYD_NET_CONNECTED_SOCKET
#define INCLUDED_SYD_NET_CONNECTED_SOCKET

#include <address.h>
#include <socket.h>

#include <cstddef>

#include <system_error>

#include <gsl/span>

namespace syd {
namespace net {

class connected_socket
{
  bool                 d_okay             = true;
  bool                 d_wouldBlock       = false;
  int                  d_fd               = -1;
  size_t               d_lastBytesWritten = 0;
  size_t               d_lastBytesRead    = 0;

  mutable std::error_condition d_lastError;

public:
  /**
   * Create a socket with the specified 'type' connected to the specified
   * 'remoteAddress' and optionally bound to the specified 'localAddress'.
   */
  connected_socket(type           type,
                   address const &remoteAddress,
                   address const &localAddress = address());

  /**
   * Write the specified 'data' to the socket. See 'write_count' for the number
   * of bytes written. If an error occurs the socket will compare to 'false'
   * after this call and 'error' will return an appropriate error condition.
   */
  connected_socket &write(gsl::span<const char> data);

  /**
   * Read the specified 'data' from the socket. See 'read_count' for the number
   * of bytes read. If an error occurs the socket will compare to 'false' after
   * this call and 'error' will return an appropriate error condition.
   */
  connected_socket &read(gsl::span<char> data);

  /**
   * Return 'false' when an error has occured, 'true' otherwise.
   */
  operator bool() const;

  /**
   * Return the number of bytes wittten.
   */
  size_t write_count() const;

  /**
   * Return the socket local address.
   */
  address local_address() const;

  /**
   * Return 'true' if the previous 'write' or 'read' would have blocked.
   */
  bool would_block() const;
  
  /**
   * Return the current error condition.
   */
  std::error_condition const &error() const;
};

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_CONNECTED_SOCKET
