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

  std::vector<Cell> Desc;

  uint32_t getFirstFreeIdx() const {
    auto FirstFree = std::find_if(Desc.begin(), Desc.end(), [](Cell const &C) {
      return C == Cell::FREE;
    });
    // TODO check if there is no free cell.
    return std::distance(Desc.begin(), FirstFree);
  }

  uint32_t getFirstFreeIdx(uint32_t From) const {
    // TODO validate input.
    auto FirstFree =
        std::find_if(Desc.begin() + From, Desc.end(),
                     [](Cell const &C) { return C == Cell::FREE; });
    // TODO check if there is no free cell.
    return std::distance(Desc.begin(), FirstFree);
  }

  uint32_t getFirstFreeIdx(uint32_t From, uint32_t Till) const {
    // TODO validate input.
    auto FirstFree =
        std::find_if(Desc.begin() + From, Desc.begin() + Till,
                     [](Cell const &C) { return C == Cell::FREE; });
    // TODO check if there is no free cell.
    return std::distance(Desc.begin(), FirstFree);
  }

public:
  CellDescription(uint32_t SizeIn)
      : Desc{std::vector<Cell>{SizeIn, Cell::OCCUPIED}} {
    std::cerr << "Allocating Description: " << Desc.size() << std::endl;
  }

  Cell const &at(uint32_t Pos) const {
    // TODO assert checks Size
    std::cerr << "Getting info about line: " << Pos << std::endl;
    return Desc.at(Pos);
  }

  uint32_t getLineIdx(uint32_t Pos) const;
  uint32_t occupyLine(uint32_t Pos);

  std::ostream &Print(std::ostream &os) const {
    for (auto const &C : Desc)
      PrintCell(os, C) << ' ';
    return os;
  }
};

class MemoryManager {
  friend CellDescription;
  void *Buffer = nullptr;

  // This struct only need for address calculating.
  // The smallest line entity is the Line<2> and all calculating are in terms of
  // Line<2>.
  // TODO get rid of DummyLine.
  struct DummyLine {
    Point P[2];
    DummyLine() = delete;
  };

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
    static_assert(Width != 1 && "Line<1> is prohibited.");
    uint32_t constexpr Row = getPowOfTwo<Width>() - 1;

    uint32_t const DescIdx = CD.getFirstFreeIdx(Row);
    uint32_t const LineIdx = CD.occupyLine(DescIdx);

    std::cout << LineIdx << std::endl;
    return Line<Width>::createLine(nullptr);
  }

  ~MemoryManager();
};

} // namespace nmgr
