#pragma once

#include <nmgr/point.h>

namespace nmgr {

template <uint32_t N>
concept LegalLine = HasSingleBit<N> && NotOne<N>;

class MemoryManager;
class LineBase {};

template <uint32_t Width>
requires LegalLine<Width>
class Line : public LineBase {
  friend MemoryManager;
  // Const data and const pointer.
  Point const *const Points = nullptr;

  static unsigned constexpr Size = Width;
  Line(Point const *P) : Points{P} {}

public:
  static Line createLine(Point const *P) { return Line{P}; }
};

} // namespace nmgr