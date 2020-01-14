// connected_writer.cpp                                               -*-c++-*-
#include <connected_writer.h>

#include <unistd.h>

namespace syd {
namespace net {

connected_writer &connected_writer::write(gsl::span<const char> data)
{
  connected_socket::handle_type fd = d_socket.native_handle();

  int res = ::write(fd, data.data(), data.size());
  if (-1 == res) {
    d_last_error = std::generic_category().default_error_condition(errno);
    d_last_bytes = 0;
    return *this;
  }

  d_last_error = std::error_condition();
  d_last_bytes = static_cast<size_t>(res);
  
  return *this;
}

} // namespace net
} // namespace syd
