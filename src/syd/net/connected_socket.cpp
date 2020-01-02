// connected_socket.cpp -*-c++-*-
#include <connected_socket.h>

#include <cassert>
#include <unistd.h>

#include <string>

namespace {
bool checkResult(int                   resultCode,
                 bool *                okay,
                 std::error_condition *errorCondition)
{
  if (-1 == resultCode) {
    if (nullptr != okay) {
      *okay = false;
    }
    *errorCondition = std::generic_category().default_error_condition(errno);
    return false;
  }

  return true;
}
} // namespace

namespace syd {
namespace net {

connected_socket::connected_socket(type           type,
                                   address const &remoteAddress,
                                   address const &localAddress)
{
  d_fd = ::socket(remoteAddress.family(), static_cast<int>(type), 0);
  if (!checkResult(d_fd, &d_okay, &d_lastError)) {
    return;
  }

  int res = 0;
  if (localAddress != nullptr) {
    res = ::bind(d_fd, localAddress.native(), localAddress.size());
    if (!checkResult(res, &d_okay, &d_lastError)) {
      return;
    }
  }

  res = ::connect(d_fd, remoteAddress.native(), remoteAddress.size());
  if (!checkResult(res, &d_okay, &d_lastError)) {
    return;
  }
}

connected_socket &connected_socket::write(gsl::span<const char> data)
{
  int res = ::write(d_fd, data.data(), data.size());
  if (-1 == res) {
    if (EWOULDBLOCK != errno && EAGAIN != errno) {
      d_okay      = false;
      d_lastError = std::generic_category().default_error_condition(errno);
      return *this;
    }

    res = 0;
  }

  d_lastBytesWritten = static_cast<size_t>(res);

  return *this;
}

connected_socket &connected_socket::read(gsl::span<char> data)
{
  int res = ::read(d_fd, data.data(), data.size());
  if (-1 == res) {
    if (EWOULDBLOCK != errno && EAGAIN != errno) {
      d_okay      = false;
      d_lastError = std::generic_category().default_error_condition(errno);
      return *this;
    }

    res = 0;
  }

  d_lastBytesRead = static_cast<size_t>(res);

  return *this;
}

connected_socket::operator bool() const { return d_okay; }

size_t connected_socket::write_count() const { return d_lastBytesWritten; }

address connected_socket::local_address() const
{
  address localAddress;
  localAddress.resize(sizeof(sockaddr));

  bool loop = true;
  while (loop) {
    socklen_t len = localAddress.capacity();
    int       res = getsockname(d_fd, localAddress.native(), &len);

    if (!checkResult(res, nullptr, &d_lastError)) {
      localAddress = address();
      break;
    }

    loop = len > localAddress.capacity();

    localAddress.resize(len);
  }

  return localAddress;
}

std::error_condition const &connected_socket::error() const
{
  return d_lastError;
}

} // namespace net
} // namespace syd
