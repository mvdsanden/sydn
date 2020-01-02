// unix_address.cpp                                                   -*-c++-*-
#include <unix_address.h>

#include <sys/un.h>

#include <gtest/gtest.h>

using namespace syd;

TEST(NetUnixAddress, Construction)
{
  net::unix_address address("/tmp/test");
  EXPECT_EQ(address.family(), net::address_family::Unix);
  EXPECT_EQ(address.path(), "/tmp/test");
}

TEST(NetUnixAddress, SetPathValid)
{
  net::unix_address address("/tmp/test");
  EXPECT_EQ(address.set_path("/usr/tmp/test"), true);
  EXPECT_EQ(address.path(), "/usr/tmp/test");
}

TEST(NetUnixAddress, SetPathInvalid)
{
  net::unix_address address("/tmp/test");

  std::string path(sizeof(sockaddr_un::sun_path), 'a');
  
  EXPECT_EQ(address.set_path("/" + path), false);
}

TEST(NetUnixAddress, Print)
{
  net::unix_address address("/tmp/test");

  std::ostringstream ss;
  ss << address;

  EXPECT_EQ(ss.str(), "unix:/tmp/test");
}
