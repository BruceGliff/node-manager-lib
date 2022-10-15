#pragma once

#include <ostream>

namespace nmgr {

struct Point final {
  float X;
  float Y;
};

} // namespace nmgr

inline std::ostream &operator<<(std::ostream &os, nmgr::Point const &P) {
  os << "P: "
     << "{" << P.X << ", " << P.Y << "}";
  return os;
}
