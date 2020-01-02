#include <mem_sdo.h>

#include <gtest/gtest.h>

using namespace syd::mem;

TEST(SdoTest, DefaultConstruction)
{
  const size_t s = 16;
  sdo<s>       obj;
  EXPECT_EQ(s, obj.capacity());
  EXPECT_EQ(0, obj.size());
}

TEST(SdoTest, ConstructWithSize)
{
  const size_t s = 16;
  sdo<s>       obj(s);
  EXPECT_EQ(s, obj.capacity());
  EXPECT_EQ(s, obj.size());
}

TEST(SdoTest, ResizePlusOne)
{
  const size_t s = 16;
  sdo<s>       obj(s);
  obj.resize(s + 1);
  EXPECT_LT(s, obj.capacity());
  EXPECT_EQ(s + 1, obj.size());
}

TEST(SdoTest, ResizeMinusOne)
{
  const size_t s = 16;
  sdo<s>       obj(s);
  obj.resize(s - 1);
  EXPECT_EQ(s, obj.capacity());
  EXPECT_EQ(s - 1, obj.size());
}

TEST(SdoTest, NoReallocWhenResizeSmallerThanMinSize)
{
  const size_t s = 16;
  sdo<s>       obj(s);
  const char * ptr = obj.data();
  obj.resize(s - 1);
  EXPECT_EQ(ptr, obj.data());
}
