#pragma once

#include "PowOfTwo.h"
#include "nmgr/line.hpp"

#include <unordered_map>
#include <vector>
// TODO remove
#include <iostream>

namespace nmgr {

// TODO forward decl?
// template <unsigned Width>
// class Line;

// This class describes which cells in MemoryManager are free=0 and occupied=1;
class CellDescription {

  enum Cell : char { FREE = 0, OCCUPIED, UNDEF };

  Cell *Desc = nullptr;

public:
  CellDescription(unsigned Size) {
    std::cerr << "Allocating Description: " << Size << std::endl;
    Desc = new Cell[Size](FREE);
  }

  ~CellDescription() { delete[] Desc; }
};

class MemoryManager {

  void *Buffer = nullptr;
  CellDescription CD;

  static void *allocMemory();
  static unsigned calcSize();

public:
  MemoryManager();

  template <unsigned Width> Line<Width> createLine() {
    unsigned constexpr Row = getPowOfTwo<Width>();
    std::cout << Row << std::endl;
    return Line<Width>::createLine();
  }

  ~MemoryManager();
};

} // namespace nmgr