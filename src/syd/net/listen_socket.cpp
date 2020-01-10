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
    d_last_error = std::generic_category().default_error_condition(errno);
    return false;
  }

  return true;
}
  
// CREATORS
listen_socket::listen_socket(type type, address const &local_address)
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

  d_local_address.resize(local_address.size());
  socklen_t addressLength = d_local_address.capacity();
  res = getsockname(d_fd, d_local_address.native(), &addressLength);
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
    d_last_error = std::generic_category().default_error_condition(errno);
    return *this;
  }

  socket.import(fd);
  return *this;
}

listen_socket &listen_socket::accept(connected_socket &socket, address &address)
{
  address.resize(d_local_address.size());
  socklen_t addressLength = address.capacity();

  int fd = ::accept(d_fd, address.native(), &addressLength);
  if (-1 == fd) {
    d_last_error = std::generic_category().default_error_condition(errno);
    return *this;
  }

  socket.import(fd);
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
