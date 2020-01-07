// address_info_list.cpp                                                       -*-c++-*-
#include <address_info_list.h>

namespace syd {
namespace net {

void addrinfo_delete::operator()(addrinfo *ptr) const { freeaddrinfo(ptr); }

family address_info::family() const
{
  return static_cast<family>(d_element.ai_family);
}

type address_info::type() const
{
  return static_cast<type>(d_element.ai_type);
}

int address_info::protocol() const
{
  return d_element.ai_protocol;
}

address address_info::address() const
{
  address result;
  result.resize(d_element.ai_addrlen);
  memcpy(result.native(), d_element.ai_addr, d_element.ai_addrlen);
  return result;
}

  
};

} // namespace net
} // namespace syd
