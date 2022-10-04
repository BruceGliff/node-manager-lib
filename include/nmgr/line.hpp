#pragma once

#include <nmgr/point.h>

namespace nmgr {

class LineBase {};

template <unsigned Width>
// TODO check that Width is power of two.
class Line : public LineBase {
  // Const data and const pointer.
  Point const *const Points = nullptr;

  unsigned Size = 0;
  Line(Point const *P) : Points{P} {} //

public:
  static Line createLine(Point const *P) { return Line{P}; }
};

} // namespace nmgr