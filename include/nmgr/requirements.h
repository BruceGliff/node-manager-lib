#pragma once

#include <bit>
#include <cstdint>

namespace nmgr {
template <uint32_t N>
concept HasSingleBit = std::has_single_bit(N);

template <uint32_t N>
concept NotOne = N != 1;

template <uint32_t N>
concept LegalLine = HasSingleBit<N> && NotOne<N>;

} // namespace nmgr
