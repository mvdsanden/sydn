// connected_socket.t.cpp                                             -*-c++-*-
#include <posix_socket_buffer.h>

#include <ipv4_address.h>
#include <connected_socket.h>

#include <arpa/inet.h>

#include <gtest/gtest.h>

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

TEST(PosixSocketBuffer, Construction)
{
  net::address      serverAddress;
  std::future<net::ipv4_address> serverResult =
      createAcceptServer(&serverAddress);

  net::posix_socket_buffer socket;
  auto result = socket.connect(net::type::Stream, serverAddress);
  EXPECT_FALSE(result) << result.message();

  net::address serverReportedAddress = serverResult.get();
  net::address localAddress;
  EXPECT_FALSE(socket.local_address(localAddress));
  EXPECT_NE(localAddress, nullptr);

  EXPECT_TRUE(compareAddress(serverReportedAddress, localAddress))
      << "serverReportedAddress=" << serverReportedAddress
      << ", local_address()=" << localAddress;  
}

TEST(PosixSocketBuffer, DestructionConnect)
{
  size_t fdCount = 0;
  size_t fdCount0 = countFileDescriptorsInUse();
  
  {
    net::address      serverAddress;
    std::future<net::ipv4_address> serverResult =
        createAcceptServer(&serverAddress);

    fdCount = countFileDescriptorsInUse();
    
    net::posix_socket_buffer socket;
    auto result = socket.connect(net::type::Stream, serverAddress);
    EXPECT_FALSE(result) << result.message();

    EXPECT_GT(countFileDescriptorsInUse(), fdCount);
  }

  EXPECT_EQ(countFileDescriptorsInUse(), fdCount0);
}

TEST(PosixSocketBuffer, ConstructionLocalAddress)
{
  net::address      serverAddress;
  std::future<net::ipv4_address> serverResult =
      createAcceptServer(&serverAddress);

  net::posix_socket_buffer socket;
  auto                  result = socket.connect(
      net::type::Stream, serverAddress, net::ipv4_address("127.0.0.1", 0));

  EXPECT_FALSE(result) << result.message();

  net::address serverReportedAddress = serverResult.get();
  net::address localAddress;
  EXPECT_FALSE(socket.local_address(localAddress));
  EXPECT_NE(localAddress, nullptr);

  EXPECT_TRUE(compareAddress(serverReportedAddress, localAddress))
      << "serverReportedAddress=" << serverReportedAddress
      << ", local_address()=" << localAddress;
}

TEST(PosixSocketBuffer, WriteString)
{
  net::address      serverAddress;
  std::future<void> serverResult =
      createServerExpectString("This is a test", &serverAddress);

  net::posix_socket_buffer socket;
  auto result = socket.connect(net::type::Stream, serverAddress);
  EXPECT_FALSE(result) << result.message();

  std::error_condition error;
  size_t count;

  tie(error, count) = socket.write(std::string("This is a test"));
  EXPECT_FALSE(error) << error.message();
  EXPECT_EQ(count, 14);
}

TEST(PosixSocketBuffer, WriteChars)
{
  net::address      serverAddress;
  std::future<void> serverResult =
      createServerExpectString("This is a test", &serverAddress);

  net::posix_socket_buffer socket;
  auto result = socket.connect(net::type::Stream, serverAddress);
  EXPECT_FALSE(result) << result.message();

  std::error_condition error;
  size_t count;

  tie(error, count) = socket.write("This is a test");
  EXPECT_FALSE(error) << error.message();
  EXPECT_EQ(count, 15);
}

TEST(PosixSocketBuffer, WriteSpan)
{
  net::address      serverAddress;
  std::future<void> serverResult =
      createServerExpectString("This ", &serverAddress);

  net::posix_socket_buffer socket;
  auto result = socket.connect(net::type::Stream, serverAddress);
  EXPECT_FALSE(result) << result.message();

  std::error_condition error;
  size_t count;

  tie(error, count) = socket.write(gsl::make_span("This is a test", 5));
  EXPECT_FALSE(error) << error.message();
  EXPECT_EQ(count, 5);
}

TEST(PosixSocketBuffer, ReadBuffer)
{
  net::address      serverAddress;
  std::future<void> serverResult =
      createServerWriteString("This is a test", &serverAddress);

  net::posix_socket_buffer socket;
  auto result = socket.connect(net::type::Stream, serverAddress);
  EXPECT_FALSE(result) << result.message();

  std::error_condition error;
  size_t count;
  
  char                  buffer[1024] = {0};
  tie(error, count) = socket.read(buffer);
  EXPECT_FALSE(error) << error.message();

  EXPECT_EQ(count, 14);
  EXPECT_STREQ(buffer, "This is a test");
}

TEST(PosixSocketBuffer, ReadVector)
{
  net::address      serverAddress;
  std::future<void> serverResult =
      createServerWriteString("This is a test", &serverAddress);

  net::posix_socket_buffer socket;
  auto result = socket.connect(net::type::Stream, serverAddress);
  EXPECT_FALSE(result) << result.message();

  std::error_condition error;
  size_t count;
  
  std::vector<char> v(1024);
  tie(error, count) = socket.read(v);
  EXPECT_FALSE(error) << error.message();

  EXPECT_EQ(count, 14);
  EXPECT_STREQ(&*v.begin(), "This is a test");
}




TEST(PosixSocketBuffer, ConstructionNoAddressNoPort)
{
  net::ipv4_address localAddress("0.0.0.0", 0);

  net::posix_socket_buffer socket;
  EXPECT_FALSE(socket.listen(net::type::Stream, localAddress));
}

TEST(PosixSocketBuffer, ConstructionWithAddressNoPort)
{
  net::ipv4_address localAddress("127.0.0.1", 0);

  net::posix_socket_buffer socket;
  EXPECT_FALSE(socket.listen(net::type::Stream, localAddress));
}

TEST(PosixSocketBuffer, ConstructionNoAddressWithPort)
{
  net::ipv4_address localAddress("0.0.0.0",
                                 12345); // TODO: can fail if port in use...

  net::posix_socket_buffer socket;
  EXPECT_FALSE(socket.listen(net::type::Stream, localAddress));
}

TEST(PosixSocketBuffer, ConstructionWithAddressAndPort)
{
  net::ipv4_address localAddress("127.0.0.1",
                                 12345); // TODO: can fail if port in use...

  net::posix_socket_buffer socket;
  EXPECT_FALSE(socket.listen(net::type::Stream, localAddress));
}

TEST(PosixSocketBuffer, DestructionListen)
{
  size_t fdCount = countFileDescriptorsInUse();

  {
    net::ipv4_address localAddress("0.0.0.0", 0);

    net::posix_socket_buffer socket;
    EXPECT_FALSE(socket.listen(net::type::Stream, localAddress));
    EXPECT_GT(countFileDescriptorsInUse(), fdCount);
  }

  EXPECT_EQ(countFileDescriptorsInUse(), fdCount);
}

TEST(PosixSocketBuffer, AcceptWithAddress)
{
  net::ipv4_address localAddress("127.0.0.1", 0);

  net::posix_socket_buffer socket;
  EXPECT_FALSE(socket.listen(net::type::Stream, localAddress));

  net::address serverAddress;
  EXPECT_FALSE(socket.local_address(serverAddress));
  
  auto connectFuture =
      std::async(std::launch::async, [serverAddress]() {
        net::connected_socket remoteSocket;
        auto result = remoteSocket.connect(net::type::Stream, serverAddress);
        EXPECT_FALSE(result) << result.message();
        return remoteSocket.local_address();
      });

  net::posix_socket_buffer clientSocket;
  net::address clientAddress;

  EXPECT_FALSE(socket.accept(clientSocket, clientAddress));
  //  EXPECT_TRUE(clientSocket) << clientSocket.error().message();
  EXPECT_TRUE(compareAddress(connectFuture.get(), clientAddress));
}

TEST(PosixSocketBuffer, AcceptWithoutAddress)
{
  net::ipv4_address localAddress("127.0.0.1", 0);

  net::posix_socket_buffer socket;
  EXPECT_FALSE(socket.listen(net::type::Stream, localAddress));

  net::address serverAddress;
  EXPECT_FALSE(socket.local_address(serverAddress));
  
  auto connectFuture =
      std::async(std::launch::async, [serverAddress]() {
        net::connected_socket remoteSocket;
        auto result = remoteSocket.connect(net::type::Stream, serverAddress);
        EXPECT_FALSE(result) << result.message();
      });

  net::posix_socket_buffer clientSocket;
  EXPECT_FALSE(socket.accept(clientSocket));
  //  EXPECT_TRUE(clientSocket) << clientSocket.error().message();
}
