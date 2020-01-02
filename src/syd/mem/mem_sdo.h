// address.h                                                          -*-c++-*-
#ifndef INCLUDED_SYD_MEM_SDO
#define INCLUDED_SYD_MEM_SDO

#include <cstddef>
#include <utility>
#include <algorithm>
#include <cassert>

namespace syd {
namespace mem {

/**
 *  Small data optimized dynamic array.
 */
template <size_t _smallSize>
class sdo
{
  size_t d_capacity = _smallSize;
  size_t d_size     = 0;

  union
  {
    char *d_ptr;
    char  d_data[_smallSize] = {0};
  };

public:
  sdo(size_t size = 0) { resize(size); }

  sdo(sdo const &other) { *this = other; }

  sdo(sdo &&other) noexcept { *this = std::move(other); }

  ~sdo()
  {
    if (d_capacity > _smallSize) {
      delete[] d_ptr;
    }
  }

  sdo &operator=(sdo const &other)
  {
    resize(other.d_size);
    std::copy(other.data(), other.data() + d_size, data());
    return *this;
  }

  sdo &operator=(sdo &&other) noexcept
  {
    assert(other.d_capacity >= _smallSize);
    if (other.d_capacity == _smallSize) {
      // We need to copy. Note that we can promise noexcept in this case
      // because the capacity is equal to the small data optimization size,
      // so no need for memory allocation in the copy.
      *this = other;
      return *this;
    }

    std::swap(d_capacity, other.d_capacity);
    std::swap(d_size, other.d_size);
    std::swap(d_ptr, other.d_ptr);

    return *this;
  }

  /**
   * Resize the array to the specified 'size'.
   */
  void resize(size_t size)
  {
    d_size = size;
    if (d_capacity >= d_size) {
      return;
    }

    if (d_capacity > _smallSize) {
      delete[] d_ptr;
    }

    d_capacity = d_size;
    d_ptr      = new char[d_capacity];
  }

  size_t size() const { return d_size; }
  size_t capacity() const { return d_capacity; }

  /**
   * Return pointer to the data.
   *
   * Behavior is undefined unless size is higher than zero.
   */
  char const *data() const { return d_capacity <= _smallSize ? d_data : d_ptr; }

  /**
   * Return pointer to the data.
   *
   * Behavior is undefined unless size is higher than zero.
   */
  char *data() { return d_capacity <= _smallSize ? d_data : d_ptr; }
};

} // namespace mem
} // namespace syd

#endif // INCLUDED_SYD_MEM_SDO
