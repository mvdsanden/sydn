// connected_writer.h                                                 -*-c++-*-
#ifndef INCLUDED_SYD_NET_CONNECTED_WRITER
#define INCLUDED_SYD_NET_CONNECTED_WRITER

#include <connected_socket.h>

#include <gsl/span>

namespace syd {
namespace net {

/**
 * 
 */
class connected_writer
{
  using Socket = connected_socket;

  // DATA
  Socket &                     d_socket;
  mutable std::error_condition d_last_error;
  size_t                       d_last_bytes = 0;

  connected_writer &write(gsl::span<const char> data);
  
public:
  // CREATORS

  /**
   * Create a writer that uses the specified 'socket'.
   */
  connected_writer(Socket &socket);
  
   connected_writer(connected_writer const &) = delete;
   connected_writer(connected_writer &&) = delete;
  
  // MANIPULATORS

   connected_writer &operator=(connected_writer const &) = delete;
   connected_writer &operator=(connected_writer &&) = delete;

  /**
   * Write the specified 'data' to the writer. See 'last_bytes' for the number
   * of bytes write. If an error occurs the writer will compare to 'false' after
   * this call and 'error' will return an appropriate error condition.
   */
   template <typename... Args>
   connected_writer &write(Args&&... args); // gsl::span<char> data);

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
    * Return the last number of bytes written.
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

 inline connected_writer::connected_writer(Socket &socket)
     : d_socket(socket)
 {
 }

 template <typename... Args>
 inline connected_writer &connected_writer::write(Args&&... args)//gsl::span<char> data)
 {
   //std::tie(d_last_error, d_last_bytes) = d_socket.write(data);
   //std::tie(d_last_error, d_last_bytes) = d_socket.write(gsl::make_span(std::forward<Args>(args)...));
   return write(gsl::make_span(std::forward<Args>(args)...));
 }

 inline connected_writer::operator bool() const { return okay(); }

 inline bool connected_writer::okay() const {
   return !d_last_error && !would_have_blocked();
 }

 inline size_t connected_writer::last_bytes() const { return d_last_bytes; }

 inline bool connected_writer::would_have_blocked() const
 {
   return d_last_error == std::errc::operation_would_block ||
          d_last_error == std::errc::resource_unavailable_try_again;
}

inline std::error_condition const &connected_writer::error() const
{
  return d_last_error;
}

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_CONNECTED_WRITER
