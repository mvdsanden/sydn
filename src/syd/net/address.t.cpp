// address.t.cpp                                                      -*-c++-*-
#include <address.h>

#include <arpa/inet.h>

#include <gtest/gtest.h>

using namespace syd;

TEST(NetAddress, DefaultConstruction)
{
  net::address address;
  EXPECT_EQ(address.size(), 0);
  EXPECT_EQ(address.family(), net::address_family::Unspecified);
  EXPECT_EQ(address.native(), nullptr);
}

TEST(NetAddress, Resize)
{
  net::address address;
  address.resize(32);
  EXPECT_EQ(address.size(), 32);
  EXPECT_EQ(address.family(), net::address_family::Unspecified);
}

TEST(NetAddress, SetAddressNatively)
{
  net::address address;
  address.resize(sizeof(sockaddr_in));

  auto addr = reinterpret_cast<sockaddr_in *>(address.native());
  addr->sin_family = AF_INET;
  addr->sin_port = htons(12345);  
  EXPECT_EQ(1, inet_pton(addr->sin_family, "127.0.0.1", &addr->sin_addr));
  
  EXPECT_EQ(address.size(), sizeof(sockaddr_in));
  EXPECT_EQ(address.family(), net::address_family::IPv4);
}

TEST(NetAddress, PrintUnspecified)
{
  net::address address;
  address.resize(32);

  std::ostringstream ss;
  ss << address;

  EXPECT_EQ(ss.str(), "Unspecified address");  
}

TEST(NetAddress, PrintUnknown)
{
  net::address address;
  address.resize(32);

  auto addr = reinterpret_cast<sockaddr_in *>(address.native());
  addr->sin_family = 12345;
  
  std::ostringstream ss;
  ss << address;

  EXPECT_EQ(ss.str(), "Unknown address family: 12345");  
}


// #include <ipv4_address.h>

// int main(int argc, char *argv[]) {
//   syd::net::address addr;

//   std::cout << addr << std::endl;

//   addr = syd::net::ipv4_address("10.0.0.1", 80);

//   std::cout << addr << std::endl;

//   // auto ipv4Address = syd::net::address_cast<syd::net::ipv4_address>(addr);
//   // auto ipv4Address1 = syd::net::address_cast<syd::net::ipv4_address*>(&addr);
//   // auto ipv4Address2 = syd::net::address_cast<syd::net::ipv4_address>(syd::net::address());

//   // //  auto ipv4Address2 = syd::net::address_cast<syd::net::ipv4_address*>(&addr);

//   // std::cout << ipv4Address << std::endl;
//   // std::cout << *ipv4Address1 << std::endl;
//   // std::cout << ipv4Address2 << std::endl;
  
//   return 0;
// }
