#pragma once

#include "PowOfTwo.h"
#include "nmgr/line.hpp"

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>

// TODO remove it
#include <iostream>

namespace nmgr {

class MemoryManager {

  // Compile-time known information about line alignment.
  template <uint32_t Width> struct Info {
    // This struct only need for address calculating.
    // The smallest line entity is the Line<2> and all calculating are in terms
    // of Line<2>.
    // TODO get rid of DummyLine.
    struct DummyLine {
      Point P[2];
      DummyLine() = delete;
    };
    // Min line is Line<2> which contains two points.
    static constexpr uint32_t MaxLineWidth = Width;
    // Number of different types of Line.
    static constexpr uint32_t NumOfLineWidths = getPowOfTwo<MaxLineWidth>();
    // Description size.
    static constexpr uint32_t TotalLines = (1u << NumOfLineWidths) - 1u;
    // Number of Min lines inside one Max line.
    static constexpr uint32_t MinInMax = MaxLineWidth >> 1;
    // Total number of point to which memory is allocated.
    static constexpr uint32_t TotalPoints = MaxLineWidth * NumOfLineWidths;
  };
  // 1 << 20 ~ 170MB data
  // 1 << 24 ~ 3GB data
  // 1 << 26 ~ 14GB data, but already large address range.
  // 1 << 27 - cannot alloc so much.
  using I = Info<(1u << 5)>;

  class CellDescription {
    enum class Cell : uint8_t { FREE = 0, OCCUPIED, UNDEF };
    static std::ostream &printCell(std::ostream &os, Cell const &C);

    using Cells = std::vector<Cell>;
    Cells Desc;

    static constexpr uint32_t LineMask = (1u << I::NumOfLineWidths) - 1u;
    static constexpr uint32_t ShiftMask = LineMask ^ -1;

    template <uint32_t Width>
    uint32_t constexpr getDescIdxBeginOfCertainLine() const;

    template <uint32_t Width>
    Cells::const_iterator getDescBeginOfCertainLine() const;
    template <uint32_t Width> Cells::iterator getDescBeginOfCertainLine();

    template <uint32_t Width>
    Cells::const_iterator getDescEndOfCertainLine() const;
    template <uint32_t Width> Cells::iterator getDescEndOfCertainLine();

    template <uint32_t Width> Cells::iterator getFirstFreeCell();

    uint32_t getMemoryOffset(uint32_t DescIdx) const;

    uint32_t getMemoryOffset(Cells::const_iterator It) const;

    uint32_t getMemoryOffset(Cells::iterator It);

  public:
    CellDescription();

    template <uint32_t Width> std::optional<uint32_t> retrieveFirstFree();

    std::ostream &print(std::ostream &os) const;
  } CD;

  void *Buffer = nullptr;

  static void *allocMemory();

  uint32_t getDescIdx(Point *Pnt) const;

public:
  MemoryManager();

  template <uint32_t Width> Line<Width> createLine();

  ~MemoryManager();
};

} // namespace nmgr

#include "MemoryManager.hpp"
