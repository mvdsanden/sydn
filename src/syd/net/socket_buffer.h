// socket_buffer.h                                                    -*-c++-*-
#ifndef INCLUDED_SYD_NET_SOCKET_BUFFER
#define INCLUDED_SYD_NET_SOCKET_BUFFER

#include <address.h>
#include <socket.h>

#include <gsl/span>

#include <system_error>

namespace syd {
namespace net {

/**
 * A socked used for connected protocols, like stream sockets.
 */
class socket_buffer
{
public:
  // CREATORS

  
  // MANIPULATORS

  /**
   * Connect a socket of the specified 'type' to the specified 'remote_address'
   * optionally using the specified 'local_address'. Return an error condition.
   */
  virtual std::error_condition connect(type           type,
                                       address const &remote_address,
                                       address const &local_address = address())
  {
    return std::errc::function_not_supported;
  }

  /**
   * Listen with a socket of the specified 'type' to the specified
   * 'local_address'. Return an error condition.
   */
  virtual std::error_condition listen(type           type,
                                      address const &local_address)
  {
    return std::errc::function_not_supported;
  }

  /**
   * Accept a new connection into the specified 'socket'. Return an error
   * condition.
   */
  virtual std::error_condition accept(socket_buffer &socket)
  {
    return std::errc::function_not_supported;
  }

  /**
   * Accept a new connection into the specified 'socket'. Store the peer address
   * in the specified 'remote_address'. Return an error condition.
   */
  virtual std::error_condition accept(socket_buffer &socket,
                                      address &      remote_address)
  {
    return std::errc::function_not_supported;
  }

  /**
   * Write the specified 'data' to the socket. Return an error condition and the
   * number of bytes written.
   */
  virtual std::pair<std::error_condition, size_t>
  write(gsl::span<const char> data)
  {
    return {std::errc::function_not_supported, 0};
  }

  /**
   * Read to the specified 'data' from the socket. Return an error condition and
   * the number of bytes read.
   */
  virtual std::pair<std::error_condition, size_t>
  read(gsl::span<char> data)
  {
    return {std::errc::function_not_supported, 0};
  }
  
  // ACCESSORS

  /**
   * Retrieve the local address into the specified 'address'. Return an error
   * condition.
   */
  virtual std::error_condition local_address(address &address) const
  {
    return std::errc::function_not_supported;
  }

  /**
   * Retrieve the remote address into the specified 'address'. Return an error
   * condition.
   */
  virtual std::error_condition remote_address(address &address) const
  {
    return std::errc::function_not_supported;
  }

};

// ------------------------------ INLINE METHODS ------------------------------

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_SOCKET_BUFFER
