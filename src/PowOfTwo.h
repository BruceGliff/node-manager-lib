#pragma once

#include <bit>
#include <cstdint>

namespace nmgr {

// Return j from N=2^j
uint32_t constexpr getPowOfTwo(uint32_t N) {
  uint32_t J = 0;
  while (N > 1) {
    N >>= 1;
    ++J;
  }
  return J;
}

template <uint32_t N>
// Return j from N=2^j
uint32_t constexpr inline getPowOfTwo() {
  static_assert(std::has_single_bit(N) && "N is not power of two.");
  return getPowOfTwo(N);
}

} // namespace nmgr