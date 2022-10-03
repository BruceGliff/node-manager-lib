#pragma once

#include "PowOfTwo.h"
#include "nmgr/line.hpp"

#include <cstdint>
#include <unordered_map>
#include <vector>
// TODO remove
#include <iostream>

namespace nmgr {

// TODO forward decl?
// template <unsigned Width>
// class Line;

class MemoryManager;
// This class describes which cells in MemoryManager are free=0 and occupied=1;
class CellDescription {
  friend MemoryManager;
  enum class Cell : uint8_t { FREE = 0, OCCUPIED, UNDEF };
  static std::ostream &PrintCell(std::ostream &os, Cell const &C) {
    switch (C) {
    case Cell::FREE:
      os << "free";
      break;
    case Cell::OCCUPIED:
      os << "occu";
      break;
    default:
      os << "udef";
      break;
    }
    return os;
  }

  uint32_t const Size;
  std::vector<Cell> Desc;

public:
  CellDescription(uint32_t SizeIn)
      : Size{SizeIn}, Desc{std::vector<Cell>{Size, Cell::OCCUPIED}} {
    std::cerr << "Allocating Description: " << Desc.size() << std::endl;
  }

  Cell const &at(uint32_t Pos) const {
    // TODO assert checks Size
    std::cerr << "Getting info about line: " << Pos << std::endl;
    return Desc.at(Pos);
  }

  void Print() const {
    for (auto const &C : Desc)
      PrintCell(std::cerr, C) << ' ';
  }
};

class MemoryManager {
  void *Buffer = nullptr;

  // Min line is Line<2> which contains two points.
  static constexpr uint32_t MaxLineWidth = 16;
  static constexpr uint32_t NumOfLineWidths = getPowOfTwo<MaxLineWidth>();
  static constexpr uint32_t TotalLines = (1u << NumOfLineWidths) - 1;
  static constexpr uint32_t MinInMax = MaxLineWidth >> 1;
  static constexpr uint32_t TotalPoints = MaxLineWidth * NumOfLineWidths;

  CellDescription CD{TotalLines};

  static void *allocMemory();

  CellDescription::Cell const &getDescIdx(Point *P) const;

public:
  MemoryManager();

  template <uint32_t Width> Line<Width> createLine() {
    uint32_t constexpr Row = getPowOfTwo<Width>();
    std::cout << Row << std::endl;
    return Line<Width>::createLine();
  }

  ~MemoryManager();
};

} // namespace nmgr