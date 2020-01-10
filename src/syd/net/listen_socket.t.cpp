// listen_socket.t.cpp                                                -*-c++-*-
#include <listen_socket.h>

#include <ipv4_address.h>

#include <arpa/inet.h>

#include <gtest/gtest.h>

#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <future>
#include <sys/resource.h>

using namespace syd;

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

TEST(NetListenSocket, ConstructionNoAddressNoPort)
{
  net::ipv4_address localAddress("0.0.0.0", 0);

  net::listen_socket socket(net::type::Stream, localAddress);
  EXPECT_TRUE(socket) << socket.error().message();
}

TEST(NetListenSocket, ConstructionWithAddressNoPort)
{
  net::ipv4_address localAddress("127.0.0.1", 0);

  net::listen_socket socket(net::type::Stream, localAddress);
  EXPECT_TRUE(socket) << socket.error().message();
}

TEST(NetListenSocket, ConstructionNoAddressWithPort)
{
  net::ipv4_address localAddress("0.0.0.0",
                                 12345); // TODO: can fail if port in use...

  net::listen_socket socket(net::type::Stream, localAddress);
  EXPECT_TRUE(socket) << socket.error().message();
}

TEST(NetListenSocket, ConstructionWithAddressAndPort)
{
  net::ipv4_address localAddress("127.0.0.1",
                                 12345); // TODO: can fail if port in use...

  net::listen_socket socket(net::type::Stream, localAddress);
  EXPECT_TRUE(socket) << socket.error().message();
}

TEST(NetListenSocket, Destruction)
{
  size_t fdCount = countFileDescriptorsInUse();

  {
    net::ipv4_address localAddress("0.0.0.0", 0);

    net::listen_socket socket(net::type::Stream, localAddress);
    EXPECT_TRUE(socket) << socket.error().message();

    EXPECT_GT(countFileDescriptorsInUse(), fdCount);
  }

  EXPECT_EQ(countFileDescriptorsInUse(), fdCount);
}

TEST(NetListenSocket, AcceptWithAddress)
{
  net::ipv4_address localAddress("127.0.0.1", 0);

  net::listen_socket socket(net::type::Stream, localAddress);
  EXPECT_TRUE(socket) << socket.error().message();

  net::address serverAddress = socket.local_address();
  
  auto connectFuture =
      std::async(std::launch::async, [serverAddress]() {
        net::connected_socket remoteSocket;
        auto result = remoteSocket.connect(net::type::Stream, serverAddress);
        EXPECT_FALSE(result) << result.message();
        return remoteSocket.local_address();
      });

  net::connected_socket clientSocket;
  net::address clientAddress;

  socket.accept(clientSocket, clientAddress);
  EXPECT_TRUE(socket) << socket.error().message();
  //  EXPECT_TRUE(clientSocket) << clientSocket.error().message();
  EXPECT_TRUE(compareAddress(connectFuture.get(), clientAddress));
}

TEST(NetListenSocket, AcceptWithoutAddress)
{
  net::ipv4_address localAddress("127.0.0.1", 0);

  net::listen_socket socket(net::type::Stream, localAddress);
  EXPECT_TRUE(socket) << socket.error().message();

  net::address serverAddress = socket.local_address();
  
  auto connectFuture =
      std::async(std::launch::async, [serverAddress]() {
        net::connected_socket remoteSocket;
        auto result = remoteSocket.connect(net::type::Stream, serverAddress);
        EXPECT_FALSE(result) << result.message();
      });

  net::connected_socket clientSocket;

  socket.accept(clientSocket);
  EXPECT_TRUE(socket) << socket.error().message();
  //  EXPECT_TRUE(clientSocket) << clientSocket.error().message();
}
