// posix_socket_buffer.cpp                                            -*-c++-*-
#include <posix_socket_buffer.h>

#include <address.h>

#include <unistd.h>

namespace syd {
namespace net {

namespace {
const int c_listen_backlog = 128;
}
  
// PRIVATE MANIPULATORS
void posix_socket_buffer::import(int fd)
{
  if (-1 != d_fd) {
    close(d_fd);
    d_fd = -1;
  }

  d_fd = fd;  
}
  
// CREATORS
posix_socket_buffer::~posix_socket_buffer()
{
  if (-1 != d_fd) {
    close(d_fd);
    d_fd = -1;
  }  
}

// MANIPULATORS

std::error_condition
posix_socket_buffer::connect(type           type,
                             address const &remote_address,
                             address const &local_address)
{
  if (-1 != d_fd) {
    close(d_fd);
    d_fd = -1;
  }
  
  d_fd = ::socket(remote_address.family(), static_cast<int>(type), 0);
  if (-1 == d_fd) {
    return std::generic_category().default_error_condition(errno);
  }
  
  int res = 0;
  if (local_address != nullptr) {
    res = ::bind(d_fd, local_address.native(), local_address.size());
    if (-1 == res) {
      return std::generic_category().default_error_condition(errno);
    }
  }

  res = ::connect(d_fd, remote_address.native(), remote_address.size());
  if (-1 == res) {
    return std::generic_category().default_error_condition(errno);
  }

  return std::error_condition(); // success
}

std::error_condition
posix_socket_buffer::listen(type type, address const &local_address)
{
  assert(local_address != nullptr);
  
  d_fd = ::socket(local_address.family(), static_cast<int>(type), 0);
  if (-1 == d_fd) {
    return std::generic_category().default_error_condition(errno);
  }

  int res = ::bind(d_fd, local_address.native(), local_address.size());
  if (-1 == res) {
    return std::generic_category().default_error_condition(errno);
  }

  res = ::listen(d_fd, c_listen_backlog);
  if (-1 == res) {
    return std::generic_category().default_error_condition(errno);
  }

  return std::error_condition(); // success  
}

std::error_condition posix_socket_buffer::accept(socket_buffer &socket)
{
  auto posix_socket = dynamic_cast<posix_socket_buffer *>(&socket);
  assert(nullptr != posix_socket);
  
  int fd = ::accept(d_fd, nullptr, nullptr);

  if (-1 == fd) {
    return std::generic_category().default_error_condition(errno);
  }

  posix_socket->import(fd);
  return std::error_condition();
}

std::error_condition posix_socket_buffer::accept(socket_buffer &socket,
                                                 address &      address)
{
  auto posix_socket = dynamic_cast<posix_socket_buffer *>(&socket);
  assert(nullptr != posix_socket);

  address.resize(sizeof(sockaddr)); // TODO: improve?
  socklen_t addressLength = address.capacity();

  int fd = ::accept(d_fd, address.native(), &addressLength);
  if (-1 == fd) {
    return std::generic_category().default_error_condition(errno);
  }

  posix_socket->import(fd);
  address.resize(addressLength);

  if (addressLength > address.capacity()) {
    // The provided address length was to small, retry with more space.
    int res = getsockname(d_fd, address.native(), &addressLength);    
    if (-1 == res) {
      address.clear();
    }
  }

  return std::error_condition();
}
  
std::pair<std::error_condition, size_t>
posix_socket_buffer::write(gsl::span<const char> data)
{
  int res = ::write(d_fd, data.data(), data.size());
  if (-1 == res) {
    return {std::generic_category().default_error_condition(errno), 0};
  }

  assert(res >= 0);
  return {std::error_condition(), static_cast<size_t>(res)};
}

std::pair<std::error_condition, size_t>
posix_socket_buffer::read(gsl::span<char> data)
{
  int res = ::read(d_fd, data.data(), data.size());
  if (-1 == res) {
    return {std::generic_category().default_error_condition(errno), 0};
  }

  assert(res >= 0);
  return {std::error_condition(), static_cast<size_t>(res)};
}

// ACCESSORS
std::error_condition posix_socket_buffer::local_address(address &address) const
{
  address.resize(sizeof(sockaddr));

  bool loop = true;
  while (loop) {
    socklen_t len = address.capacity();
    int       res = getsockname(d_fd, address.native(), &len);

    if (-1 == res) {
      address = net::address{};
      return std::generic_category().default_error_condition(errno);
    }

    loop = len > address.capacity();

    address.resize(len);
  }

  return std::error_condition();
}

// std::error_condition
// posix_socket_buffer::remote_address(address &address) const
// {
//   // TODO: implement.
// }

} // namespace net
} // namespace syd
