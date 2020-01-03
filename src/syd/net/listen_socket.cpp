// listen_socket.cpp                                                  -*-c++-*-
#include <listen_socket.h>

#include <unistd.h>

namespace {
const int c_listen_backlog = 128;
}

namespace syd {
namespace net {

// PRIVATE MANIPULATORS
bool listen_socket::check_result(int result_code) const
{
  if (-1 == result_code) {
    d_okay       = false;
    d_last_error = std::generic_category().default_error_condition(errno);
    return false;
  }

  return true;
}
  
// CREATORS
listen_socket::listen_socket(type type, address const &local_address)
    : d_okay(true)
    , d_would_have_blocked(false)
{
  assert(local_address != nullptr);
  
  d_fd = ::socket(local_address.family(), static_cast<int>(type), 0);
  if (!check_result(d_fd)) {
    return;
  }

  int res = ::bind(d_fd, local_address.native(), local_address.size());
  if (!check_result(res)) {
    return;
  }

  res = ::listen(d_fd, c_listen_backlog);
  if (!check_result(res)) {
    return;
  }
}

listen_socket::~listen_socket()
{
  if (-1 != d_fd) {
    close(d_fd);
  }
}

// MANIPULATORS
listen_socket &listen_socket::accept(connected_socket &socket)
{
  int fd = ::accept(d_fd, nullptr, nullptr);

  if (-1 == fd) {
    if (EWOULDBLOCK == errno || EAGAIN == errno) {
      d_would_have_blocked = true;
      return *this;
    }

    d_okay       = false;
    d_last_error = std::generic_category().default_error_condition(errno);
    return *this;
  }

  socket.initialize_from_fd(fd);
  return *this;
}

listen_socket &listen_socket::accept(connected_socket &socket, address &address)
{
  address.reserve(d_local_address.size());
  socklen_t addressLength = address.capacity();

  int fd = ::accept(d_fd, address.native(), &addressLength);
  if (-1 == fd) {
    if (EWOULDBLOCK == errno || EAGAIN == errno) {
      d_would_have_blocked = true;
      return *this;
    }

    d_okay       = false;
    d_last_error = std::generic_category().default_error_condition(errno);
    return *this;
  }

  socket.initialize_from_fd(fd);
  address.resize(addressLength);

  if (addressLength > address.capacity()) {
    // The provided address length was to small, retry with more space.
    int res = getsockname(d_fd, address.native(), &addressLength);    
    if (-1 == res) {
      address.clear();
    }
  }

  return *this;
}

// ACCESSORS

} // namespace net
} // namespace syd
