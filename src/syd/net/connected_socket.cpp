// connected_socket.cpp                                               -*-c++-*-
#include <connected_socket.h>

#include <cassert>
#include <string>
#include <unistd.h>

namespace syd {
namespace net {

// PRIVATE MANIPULATORS

// CREATORS
connected_socket::connected_socket() = default;

connected_socket::~connected_socket()
{
  if (-1 != d_fd) {
    close(d_fd);
  }
}

// MANIPULATORS
std::error_condition connected_socket::connect(type           type,
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

void connected_socket::import(int fd)
{
  if (-1 != d_fd) {
    close(d_fd);
  }

  d_fd = fd;
}
  
std::pair<std::error_condition, size_t>
connected_socket::write(gsl::span<const char> data)
{
  int res = ::write(d_fd, data.data(), data.size());
  if (-1 == res) {
    return {std::generic_category().default_error_condition(errno), 0};
  }

  assert(res >= 0);
  return {std::error_condition(), static_cast<size_t>(res)};
}

std::pair<std::error_condition, size_t>
connected_socket::read(gsl::span<char> data)
{
  int res = ::read(d_fd, data.data(), data.size());
  if (-1 == res) {
    return {std::generic_category().default_error_condition(errno), 0};
  }

  assert(res >= 0);
  return {std::error_condition(), static_cast<size_t>(res)};
}

// ACCESSORS
address connected_socket::local_address() const
{
  address localAddress;
  localAddress.resize(sizeof(sockaddr));

  bool loop = true;
  while (loop) {
    socklen_t len = localAddress.capacity();
    int       res = getsockname(d_fd, localAddress.native(), &len);

    if (-1 == res) {
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
