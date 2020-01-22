// posix_socket_buffer.h                                              -*-c++-*-
#ifndef INCLUDED_SYD_NET_POSIX_SOCKET_BUFFER
#define INCLUDED_SYD_NET_POSIX_SOCKET_BUFFER

#include <socket_buffer.h>

namespace syd {
namespace net {

/**
 * Posix socket buffer implementation.
 */
class posix_socket_buffer : public socket_buffer
{
  // DATA
  int d_fd = -1;

  // PRIVATE MANIPULATORS
  void import(int fd);

public:
  // CREATORS
  virtual ~posix_socket_buffer();
  
  // MANIPULATORS

  /**
   * Connect a socket of the specified 'type' to the specified 'remote_address'
   * optionally using the specified 'local_address'. Return an error condition.
   */
  std::error_condition
  connect(type           type,
          address const &remote_address,
          address const &local_address = address()) override;

  /**
   * Listen with a socket of the specified 'type' to the specified
   * 'local_address'. Return an error condition.
   */
  std::error_condition listen(type type, address const &local_address) override;
  /**
   * Accept a new connection into the specified 'socket'. Return an error
   * condition.
   */
  std::error_condition accept(socket_buffer &socket) override;

  /**
   * Accept a new connection into the specified 'socket'. Store the peer address
   * in the specified 'remote_address'. Return an error condition.
   */
  std::error_condition accept(socket_buffer &socket,
                              address &      remote_address) override;

  /**
   * Write the specified 'data' to the socket. Return an error condition and the
   * number of bytes written.
   */
  std::pair<std::error_condition, size_t>
  write(gsl::span<const char> data) override;

  /**
   * Read to the specified 'data' from the socket. Return an error condition and
   * the number of bytes read.
   */
  std::pair<std::error_condition, size_t> read(gsl::span<char> data) override;

  // ACCESSORS

  /**
   * Retrieve the local address into the specified 'address'. Return an error
   * condition.
   */
  std::error_condition local_address(address &address) const override;

  /**
   * Retrieve the remote address into the specified 'address'. Return an error
   * condition.
   */
  //  std::error_condition remote_address(address &address) const override;
};

// ------------------------------ INLINE METHODS ------------------------------

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_POSIX_SOCKET_BUFFER
