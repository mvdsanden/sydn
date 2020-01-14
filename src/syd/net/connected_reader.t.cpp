// connected_socket.t.cpp -*-c++-*-
#include <connected_reader.h>

#include <connected_socket.h>
#include <ipv4_address.h>

#include <gtest/gtest.h>

#include <arpa/inet.h>

#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <future>
#include <sys/resource.h>

using namespace syd;

int createListeningSocket(net::address *address)
{
  assert(nullptr != address);

  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  assert(-1 != fd);

  net::ipv4_address localAddress("127.0.0.1", 0);
  int res = ::bind(fd, localAddress.native(), localAddress.size());
  assert(-1 != res);

  res = ::listen(fd, 1);
  assert(-1 != res);

  socklen_t len = localAddress.size();
  res           = getsockname(fd, localAddress.native(), &len);
  assert(-1 != res);

  *address = localAddress;

  return fd;
}

std::string readAll(int fd, size_t size)
{
  char buffer[size];

  size_t count = 0;

  while (count != size) {
    int res = ::read(fd, buffer + count, size - count);

    if (0 > res) {
      if (EWOULDBLOCK == errno || EAGAIN == errno) {
	sleep(0);
	continue;
      }

      assert(false);
    }

    count += res;
  }

  return std::string(buffer, size);
}

void writeAll(int fd, std::string const &str)
{
  size_t count = 0;

  while (count != str.size()) {
    int res = ::write(fd, str.c_str() + count, str.size() - count);

    if (0 > res) {
      if (EWOULDBLOCK == errno || EAGAIN == errno) {
	sleep(0);
	continue;
      }

      assert(false);
    }

    count += res;
  }
}

std::future<net::ipv4_address>
createAcceptServer(net::address *serverAddress,
                   std::function<void(net::ipv4_address const &)> cb = nullptr)
{
  int server = createListeningSocket(serverAddress);
  return std::async(std::launch::async, [server]() -> net::ipv4_address {
    net::ipv4_address address;
    socklen_t len = address.size();
    int remoteSocket = ::accept(server, address.native(), &len);
    assert(-1 != remoteSocket);
    close(server);
    close(remoteSocket);
    return address;
  });
}

std::future<void> createServerExpectString(std::string const &expectString,
                                           net::address *     serverAddress)
{
  int server = createListeningSocket(serverAddress);
  return std::async(std::launch::async, [server, expectString]() {
    int remoteSocket = ::accept(server, nullptr, nullptr);
    assert(-1 != remoteSocket);
    close(server);
    std::string str = readAll(remoteSocket, expectString.size());
    EXPECT_EQ(str, expectString.c_str());
    close(remoteSocket);
  });
}

std::future<void> createServerWriteString(std::string const &str,
                                          net::address *     serverAddress)
{
  int server = createListeningSocket(serverAddress);
  return std::async(std::launch::async, [server, str]() {
    int remoteSocket = ::accept(server, nullptr, nullptr);
    assert(-1 != remoteSocket);
    close(server);
    writeAll(remoteSocket, str);
    close(remoteSocket);
  });
}

bool compareAddress(net::address const &lhs, net::address const &rhs)
{
  if (lhs.family() != rhs.family()) {
    return false;
  }

  size_t size = std::min(lhs.size(), rhs.size());

  return 0 == std::memcmp(lhs.native(), rhs.native(), size);
}

size_t countFileDescriptorsInUse()
{
  size_t count = 0;

  rlimit limit;  
  int res = getrlimit(RLIMIT_NOFILE, &limit);
  assert(0 == res);

  for (int i = 0; i < limit.rlim_max; ++i) {
    count += (fcntl(i, F_GETFD) != -1 ? 1 : 0);
  }

  return count;
}

std::pair<std::error_condition, size_t>
read(net::connected_socket const &socket, gsl::span<char> data)
{
  int res = ::read(socket.native_handle(), data.data(), data.size());
  if (-1 == res) {
    return {std::generic_category().default_error_condition(errno), 0};
  }

  assert(res >= 0);
  return {std::error_condition(), static_cast<size_t>(res)};
}

std::pair<std::error_condition, size_t>
write(net::connected_socket const &socket, gsl::span<const char> data)
{
  int res = ::write(socket.native_handle(), data.data(), data.size());
  if (-1 == res) {
    return {std::generic_category().default_error_condition(errno), 0};
  }

  assert(res >= 0);
  return {std::error_condition(), static_cast<size_t>(res)};
}

TEST(NetConnectedSocket, Constructor)
{
    net::connected_socket socket;
    net::connected_reader reader(socket);

    EXPECT_TRUE(reader);
    EXPECT_EQ(reader.last_bytes(), 0);
    EXPECT_FALSE(reader.error());
}

TEST(NetConnectedSocket, Error)
{
  net::connected_socket socket;
  net::connected_reader reader(socket);

  EXPECT_TRUE(reader);
  EXPECT_EQ(reader.last_bytes(), 0);
  EXPECT_FALSE(reader.error());

  char buffer[1024] = {0};
  EXPECT_FALSE(reader.read(buffer)) << reader.error().message();
  EXPECT_TRUE(reader.error() == std::errc::bad_file_descriptor) << reader.error().message();
}

TEST(NetConnectedSocket, ReadBuffer)
{
  net::address      serverAddress;
  std::future<void> serverResult =
      createServerWriteString("This is a test", &serverAddress);

  net::connected_socket socket;
  auto result = socket.connect(net::type::Stream, serverAddress);
  EXPECT_FALSE(result) << result.message();

  net::connected_reader reader(socket);

  char buffer[1024] = {0};
  EXPECT_TRUE(reader.read(buffer)) << reader.error().message();

  EXPECT_EQ(reader.last_bytes(), 14);
  EXPECT_STREQ(buffer, "This is a test");
}

TEST(NetConnectedSocket, ReadVector)
{
  net::address      serverAddress;
  std::future<void> serverResult =
      createServerWriteString("This is a test", &serverAddress);

  net::connected_socket socket;
  auto result = socket.connect(net::type::Stream, serverAddress);
  EXPECT_FALSE(result) << result.message();

  net::connected_reader reader(socket);
  
  std::vector<char> v(1024);
  EXPECT_TRUE(reader.read(v)) << reader.error().message();

  EXPECT_EQ(reader.last_bytes(), 14);
  EXPECT_STREQ(&*v.begin(), "This is a test");
}
