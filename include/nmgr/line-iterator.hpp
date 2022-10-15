#pragma once

#include <iterator>

namespace nmgr {

template <typename T> struct LineIt final {
  using iterator_category = std::contiguous_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = T *;
  using reference = T &;

  LineIt(pointer p_in) : p{p_in} {}

  reference operator*() const { return *p; }
  pointer operator->() { return p; }

  LineIt &operator++() {
    p++;
    return *this;
  }
  LineIt operator++(int) {
    LineIt Tmp = *this;
    ++(*this);
    return Tmp;
  }

  friend bool operator==(const LineIt &a, const LineIt &b) {
    return a.p == b.p;
  };
  friend bool operator!=(const LineIt &a, const LineIt &b) {
    return !(a == b);
  };

private:
  pointer p;
};

} // namespace nmgr