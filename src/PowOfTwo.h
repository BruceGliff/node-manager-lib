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
// requires HasSingleBit<N>
uint32_t constexpr inline getPowOfTwo() {
  return getPowOfTwo(N);
}

template <uint32_t N>
concept HasSingleBit = std::has_single_bit(N);

template <uint32_t N>
concept NotOne = N != 1;

template <uint32_t N>
concept LegalLine = HasSingleBit<N> && NotOne<N>;

} // namespace nmgr