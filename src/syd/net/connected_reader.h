// connected_reader.h                                                 -*-c++-*-
#ifndef INCLUDED_SYD_NET_CONNECTED_READER
#define INCLUDED_SYD_NET_CONNECTED_READER

#include <connected_socket.h>

#include <gsl/span>

namespace syd {
namespace net {

/**
 * 
 */
class connected_reader
{
  using Socket = connected_socket;

  // DATA
  Socket &                     d_socket;
  mutable std::error_condition d_last_error;
  size_t                       d_last_bytes = 0;
  
public:
  // CREATORS

  /**
   * Create a reader that uses the specified 'socket'.
   */
  connected_reader(Socket &socket);
  
   connected_reader(connected_reader const &) = delete;
   connected_reader(connected_reader &&) = delete;
  
  // MANIPULATORS

   connected_reader &operator=(connected_reader const &) = delete;
   connected_reader &operator=(connected_reader &&) = delete;

  /**
   * Read the specified 'data' from the reader. See 'read_count' for the number
   * of bytes read. If an error occurs the reader will compare to 'false' after
   * this call and 'error' will return an appropriate error condition.
   */
  connected_reader &read(gsl::span<char> data);

  // TODO: operator()() for reading?
  
  // ACCESSORS

  /**
   * Return 'false' when an error has occured, 'true' otherwise. Note that this
   * returns 'true' if the operation would have blocked (see
   * 'would_have_blocked').
   */
  operator bool() const;

  /**
   * Return 'false' when an error has occured, 'true' otherwise. Note that this
   * returns 'true' if the operation would have blocked (see
   * 'would_have_blocked').
   */
  bool okay() const;

  /**
   * Return the last number of bytes read.
   */
  size_t last_bytes() const;

  /**
   * Return 'true' if the previous operation would have blocked.
   */
  bool would_have_blocked() const;

  /**
   * Return the current error condition.
   */
  std::error_condition const &error() const;
};

// ------------------------------ INLINE METHODS ------------------------------

 inline connected_reader::connected_reader(Socket &socket)
     : d_socket(socket)
 {
 }

 inline connected_reader &connected_reader::read(gsl::span<char> data)
 {
   std::tie(d_last_error, d_last_bytes) = d_socket.read(data);
   return *this;
 }

 inline connected_reader::operator bool() const { return okay(); }

 inline bool connected_reader::okay() const {
   return !d_last_error && !would_have_blocked();
 }

 inline size_t connected_reader::last_bytes() const { return d_last_bytes; }

 inline bool connected_reader::would_have_blocked() const
 {
   return d_last_error == std::errc::operation_would_block ||
          d_last_error == std::errc::resource_unavailable_try_again;
}

inline std::error_condition const &connected_reader::error() const
{
  return d_last_error;
}

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_CONNECTED_READER
