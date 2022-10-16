#pragma once

#include <ostream>

namespace nmgr {

struct Point final {
  float X;
  float Y;
  static float length_sq(Point const &P1, Point const &P2) {
    float const dx = P1.X - P2.X;
    float const dy = P1.Y - P2.Y;
    return dx * dx + dy * dy;
  }
};

} // namespace nmgr

inline std::ostream &operator<<(std::ostream &os, nmgr::Point const &P) {
  os << "P: "
     << "{" << P.X << ", " << P.Y << "}";
  return os;
}
