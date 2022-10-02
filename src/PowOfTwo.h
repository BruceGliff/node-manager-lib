#pragma once

namespace nmgr {

// Return j from N=2^j
unsigned constexpr getPowOfTwo(unsigned N) {
  unsigned J = 0;
  while (N > 1) {
    N >>= 1;
    ++J;
  }
  return J;
}

template <unsigned N>
// Return j from N=2^j
unsigned constexpr inline getPowOfTwo() {
  static_assert(N && "N cannot be 0.");
  static_assert((N & -N) == N && "N is not power of two.");

  return getPowOfTwo(N);
}

} // namespace nmgr