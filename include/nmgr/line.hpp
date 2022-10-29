#pragma once

#include "MemoryManager.h"
#include "line-iterator.hpp"
#include "point.h"

#include <algorithm>
#include <ranges>
#include <span>
#include <variant>

// TODO remove
#include <iostream>

namespace nmgr {

template <uint32_t N>
concept LegalLine = HasSingleBit<N> && NotOne<N>;

class MemoryManager;
class LineBase {
public:
  virtual LineBase &append(Point &&P) = 0;
};

template <uint32_t Width>
  requires LegalLine<Width>
class Line final : public LineBase {
  friend MemoryManager;

  using iterator = LineIt<Point>;
  using const_iterator = LineIt<Point const>;

  Point *const Pts = nullptr;
  uint32_t Size = 0u;

  // TODO find more faster way to do it.
  const_iterator closest(Point const &P) const {
    // Calculating closest point in line to Input.
    std::pair<Point const *, float> ClosestP;
    std::for_each(begin(), end(), [&ClosestP, &P](Point const &PIn) {
      float const CheckDiff = Point::length_sq(P, PIn);
      if (CheckDiff > ClosestP.second)
        return;
      ClosestP = std::make_pair(&PIn, CheckDiff);
    });
    return ClosestP.first;
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
  LineBase &append(Point &&P) override {
    if (Size >= Capacity) {
      std::cout << "Line is full\n";
      return *this;
    }
    Pts[Size++] = std::move(P);
    return *this;
  }

  // Finds the closest point in the Line.
  // If the closest point is the end -> appends Line.
  // If the closest point is the begin -> reverse and append.
  // If the closest point in the middle ->
  //   creates joint, creates line, returns new created line.
  Line<2> insert(Point &&P) {
    const_iterator Closest = closest(P);

    // This logic is kinda stupid. Believe I should get rid of template.
    Line<2> Ret;
    LineBase *LB = this;
    if (Closest == begin()) {
      // Think it is better for huge lines to create new line, when to extend
      // backward.
      std::cout << "reverse\n";
    } else if (Closest != end()) {
      std::cout << "creating joint\n";
      // TODO from manager. include problem
      Ret = singleton<MemoryManager>::getInstance().createLine();
      Ret = Line<2>::createLine(nullptr);
      LB = &Ret;
    }

    // It will append or this line, or new created one.
    LB->append(std::move(P));

    return Ret;
  }

  std::span<Point> getView() const { return std::span<Point>(Pts, Size); }
};

} // namespace nmgr
