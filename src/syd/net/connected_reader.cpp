// connected_reader.cpp                                               -*-c++-*-
#include <connected_reader.h>

#include <unistd.h>

namespace syd {
namespace net {

connected_reader &connected_reader::read(gsl::span<char> data)
{
  Socket::handle_type fd = d_socket.native_handle();

  int res = ::read(fd, data.data(), data.size());
  if (-1 == res) {
    d_last_error = std::generic_category().default_error_condition(errno);
    d_last_bytes = 0;
    return *this;
  }

  assert(res >= 0);
  d_last_bytes = static_cast<size_t>(res);
  d_last_error.clear();
  return *this;
}

} // namespace net
} // namespace syd
