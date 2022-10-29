#pragma once

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
// requires HasSingleBit<N>
uint32_t constexpr inline getPowOfTwo() {
  return getPowOfTwo(N);
}

} // namespace nmgr