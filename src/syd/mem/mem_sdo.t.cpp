#include <mem_sdo.h>

#include <cassert>

using namespace mem;

void test1()
{
  const size_t s = 16;
  sdo<s> obj;
  assert(s == obj.capacity());
  assert(0 == obj.size());
}

void test2()
{
  const size_t s = 16;
  sdo<s> obj(s);
  assert(s == obj.capacity());
  assert(s == obj.size());
}

void test3()
{
  const size_t s = 16;
  sdo<s> obj(s);
  obj.resize(s+1);
  assert(s < obj.capacity());
  assert(s+1 == obj.size());
}

void test3()
{
  const size_t s = 16;
  sdo<s> obj(s);
  obj.resize(s-1);
  assert(s == obj.capacity());
  assert(s-1 == obj.size());
}

void test3()
{
  const size_t s = 16;
  sdo<s> obj;
  const char *ptr = obj.data();
  obj.resize(s-1);
  assert(ptr == obj.data());
}

int main(int argc, char **argv)
{
  return 0;
}
