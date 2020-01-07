// address_info_list.h                                                         -*-c++-*-
#ifndef INCLUDED_SYD_NET_ADDRESS_INFO_LIST
#define INCLUDED_SYD_NET_ADDRESS_INFO_LIST

#include <socket.h>
#include <address_info_list.h>

namespace syd {
namespace net {

struct addrinfo_delete
{
  void operator()(addrinfo *ptr) const;
};

/**
 * 
 */
class address_info_list
{

  std::shared_ptr<addrinfo> d_list;

public:

  class const_iterator;
  
  // CREATORS

  /**
   * 
   */
  address_info_list(addrinfo *list = nullptr);  
  
  // MANIPULATORS

  // ACCESSORS
  const_iterator begin() const;
  const_iterator end() const;  

};

class address_info
{
  std::shared_ptr<addrinfo> d_list;
  addrinfo *                d_element = nullptr;

public:  
  family family() const;
  type type() const;
  int protocol() const;
  address address() const;  
};

class address_info_list::const_iterator
{
  std::shared_ptr<addrinfo> d_list;
  addrinfo *                d_element = nullptr;

public:
  using value_type        = address_info;
  using reference         = value_type const &;
  using pointer           = value_type const *;
  using iterator_category = std::forward_iterator_tag;

  bool            operator==(const_iterator const &other) const;
  bool            operator!=(const_iterator const &other) const;
  const_iterator &operator++();
  const_iterator operator++(int);
  value_type      operator*() const;
};

// ------------------------------ INLINE METHODS ------------------------------

inline address_info_list::address_info_list(addrinfo *list = nullptr)
  : d_list(list, addrinfo_delete)
{}

inline address_info_list::const_iterator address_info_list::begin() const
{
  const_iterator iter;
  iter.d_list = d_list;
  iter.d_element = d_list.get();
  return iter;
}

inline address_info_list::const_iterator address_info_list::end() const
{
  const_iterator iter;
  iter.d_list = d_list;
  return iter;
}

inline bool address_info_list::const_iterator::
     operator==(const_iterator const &other) const
{
  return d_element == other.d_element;
}

inline bool address_info_list::const_iterator::
     operator!=(const_iterator const &other) const
{
  return d_element != other.d_element;
}

inline address_info_list::const_iterator &address_info_list::const_iterator::
                                   operator++()
{
  d_element = d_element->ai_next;
  return *this;
}

inline address_info_list::const_iterator address_info_list::const_iterator::
                                  operator++(int)
{
  const_iterator iter = *this;
  return ++iter;  
}

inline address_info_list::const_iterator::value_type
    address_info_list::const_iterator::operator*() const
{
  address_info result;
  result.d_list = d_list;
  result.d_element = d_element;
  return result;
}

} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_ADDRESS_INFO_LIST
