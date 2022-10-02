#pragma once

#include <nmgr/point.h>

namespace nmgr {

class LineBase {};

template <unsigned Width>
// TODO check that Width is power of two.
class Line : public LineBase {
  Point *Points = nullptr;

  unsigned constexpr Capacity = Width;
  unsigned Size = 0;
  Line() {} //

public:
  static createLine(/*memory manager.*/);
};

} // namespace nmgr