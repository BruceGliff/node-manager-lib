#pragma once

#include <nmgr/point.h>

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
class Line : public LineBase {
  friend MemoryManager;

  // TODO create own iterator.
  // TODO create getView() { return std::span() };
  std::span<Point const> Pts;

  Line(Point const *P) : Pts{P, 0u} {}

  static uint32_t constexpr Capacity = Width;

  Point const *getRawPts() const { return Pts.data(); }
  uint32_t getSize() const { return Pts.size(); }

public:
  static Line
  createLine(Point const *P) { /*Log about creating outside MemoryManager*/
    return Line{P};
  }

  auto begin() { return Pts.begin(); }
  auto begin() const { return Pts.begin(); }

  auto end() { return Pts.end(); }
  auto end() const { return Pts.end(); }

  Line &append(Point &&P) {
    uint32_t Size = getSize();
    if (Size >= Capacity) {
      std::cout << "Line is full\n";
      return *this;
    }
    auto *Raw = getRawPts();
    Raw[Size++] = P;

    Pts = std::span(Raw, Size);
  }
};

} // namespace nmgr