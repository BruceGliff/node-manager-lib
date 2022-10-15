#pragma once

#include "line-iterator.hpp"
#include "point.h"

#include <ranges>
#include <span>

// TODO remove
#include <iostream>

namespace nmgr {

template <uint32_t N>
concept LegalLine = HasSingleBit<N> && NotOne<N>;

class MemoryManager;
class LineBase {};

template <uint32_t Width>
  requires LegalLine<Width>
class Line final : public LineBase {
  friend MemoryManager;

  using iterator = LineIt<Point>;
  using const_iterator = LineIt<Point const>;

  Point *const Pts = nullptr;
  uint32_t Size = 0u;

  Point const *closest(Point const &P) {
    // TODO
    return Pts;
  }

  Line(Point *P) : Pts{P} {}

  static uint32_t constexpr Capacity = Width;

public:
  static Line
  createLine(Point *P) { /*Log about creating outside MemoryManager*/
    return Line{P};
  }

  operator bool() { return Pts; }
  bool empty() { return !Size; }

  iterator begin() { return Pts; }
  const_iterator begin() const { return Pts; }

  iterator end() { return Pts + Size; }
  const_iterator end() const { return Pts + Size; }

  // This just appends Point in the end of the Line.
  Line &append(Point &&P) {
    if (Size >= Capacity) {
      std::cout << "Line is full\n";
      return *this;
    }
    Pts[Size++] = P;
    return *this;
  }

  // Finds the closest point in the Line.
  // If the closest point is the end -> appends Line.
  // If the closest point is the begin -> reverse and append.
  // If the closest point in the middle ->
  //   creates joint, creates line, returns new created line.
  Line insert(Point &&P) {}

  std::span<Point> getView() const { return std::span<Point>(Pts, Size); }
};

} // namespace nmgr
