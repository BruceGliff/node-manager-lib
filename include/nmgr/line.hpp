#pragma once

#include <nmgr/point.h>

namespace nmgr {

class LineBase {};

template <unsigned Width>
// TODO check that Width is power of two.
class Line : public LineBase {
  Point *Points = nullptr;

  unsigned Size = 0;
  Line() {} //

public:
  static Line createLine(/*memory manager.*/) { return Line{}; }
};

} // namespace nmgr