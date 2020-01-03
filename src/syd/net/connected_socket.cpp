// connected_socket.cpp                                               -*-c++-*-
#include <connected_socket.h>

#include <cassert>
#include <string>
#include <unistd.h>

namespace syd {
namespace net {

bool connected_socket::check_result(int result_code) const
{
  if (-1 == result_code) {
    d_okay       = false;
    d_last_error = std::generic_category().default_error_condition(errno);
    return false;
  }

  return true;
}

connected_socket::connected_socket(type           type,
                                   address const &remote_address,
                                   address const &local_address)
    : d_okay(true)
    , d_would_have_blocked(false)
{
  d_fd = ::socket(remote_address.family(), static_cast<int>(type), 0);
  if (!check_result(d_fd)) {
    return;
  }

  int res = 0;
  if (local_address != nullptr) {
    res = ::bind(d_fd, local_address.native(), local_address.size());
    if (!check_result(res)) {
      return;
    }
  }

  res = ::connect(d_fd, remote_address.native(), remote_address.size());
  if (!check_result(res)) {
    return;
  }
}

connected_socket &connected_socket::write(gsl::span<const char> data)
{
  int res = ::write(d_fd, data.data(), data.size());
  if (-1 == res) {
    if (EWOULDBLOCK != errno && EAGAIN != errno) {
      d_okay       = false;
      d_last_error = std::generic_category().default_error_condition(errno);
      return *this;
    }

    d_would_have_blocked = true;
    res                  = 0;
  }

  d_last_bytes = static_cast<size_t>(res);

  return *this;
}

connected_socket &connected_socket::read(gsl::span<char> data)
{
  int res = ::read(d_fd, data.data(), data.size());
  if (-1 == res) {
    if (EWOULDBLOCK != errno && EAGAIN != errno) {
      d_okay       = false;
      d_last_error = std::generic_category().default_error_condition(errno);
      return *this;
    }

    res = 0;
  }

  d_last_bytes = static_cast<size_t>(res);

  return *this;
}

address connected_socket::local_address() const
{
  address localAddress;
  localAddress.resize(sizeof(sockaddr));

  bool loop = true;
  while (loop) {
    socklen_t len = localAddress.capacity();
    int       res = getsockname(d_fd, localAddress.native(), &len);

    if (!check_result(res)) {
      localAddress = address();
      break;
    }

    loop = len > localAddress.capacity();

    localAddress.resize(len);
  }

  return localAddress;
}

} // namespace net
} // namespace syd