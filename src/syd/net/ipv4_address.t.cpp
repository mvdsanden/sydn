// ipv4_address.cpp                                                   -*-c++-*-
#include <ipv4_address.h>

#include <gtest/gtest.h>

using namespace syd;

TEST(NetIpv4Address, DefaultConstructor) {
  net::ipv4_address address;
  EXPECT_EQ(address.family(), net::address_family::IPv4);
  EXPECT_EQ(address.address(), "0.0.0.0");
  EXPECT_EQ(address.port(), 0);
}

TEST(NetIpv4Address, ConstructorValid) {
  net::ipv4_address address{"10.0.0.1", 12345};
  EXPECT_EQ(address.family(), net::address_family::IPv4);
  EXPECT_EQ(address.address(), "10.0.0.1");
  EXPECT_EQ(address.port(), 12345);
}

TEST(NetIpv4Address, ConstructorInvalid) {
  net::ipv4_address address{"hello", 12345};
  EXPECT_EQ(address.family(), net::address_family::IPv4);
  EXPECT_EQ(address.address(), "0.0.0.0");
  EXPECT_EQ(address.port(), 12345);
}

TEST(NetIpv4Address, SetAddressValid) {
  net::ipv4_address address{"10.0.0.1", 0};
  EXPECT_EQ(address.set_address("10.0.1.1"), true);
  EXPECT_EQ(address.address(), "10.0.1.1");
}

TEST(NetIpv4Address, SetAddressInvalid) {
  net::ipv4_address address{"10.0.0.1", 0};
  EXPECT_EQ(address.set_address("hello"), false);
  EXPECT_EQ(address.address(), "10.0.0.1");
}

TEST(NetIpv4Address, SetPort) {
  net::ipv4_address address{"10.0.0.1", 0};
  address.set_port(12345);
  EXPECT_EQ(address.port(), 12345);
}

TEST(NetIpv4Address, Print) {
  net::ipv4_address address{"10.0.0.1", 12345};

  std::ostringstream ss;
  ss << address;

  EXPECT_EQ(ss.str(), "10.0.0.1:12345");
}
