#pragma once

#include "PowOfTwo.h"
#include "nmgr/line.hpp"

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>
// TODO remove
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
    static std::ostream &printCell(std::ostream &os, Cell const &C) {
      switch (C) {
      case Cell::FREE:
        os << "f";
        break;
      case Cell::OCCUPIED:
        os << "o";
        break;
      default:
        os << "u";
        break;
      }
      return os;
    }
    using Cells = std::vector<Cell>;
    Cells Desc;

    static constexpr uint32_t LineMask = (1u << I::NumOfLineWidths) - 1u;
    static constexpr uint32_t ShiftMask = LineMask ^ -1;

    template <uint32_t Width>
    uint32_t constexpr getDescIdxBeginOfCertainLine() const {
      uint32_t const Shift = getPowOfTwo<Width>() - 1;
      uint32_t const DescIdx = (ShiftMask >> Shift) & LineMask;
      return DescIdx;
    }

    template <uint32_t Width>
    Cells::const_iterator getDescBeginOfCertainLine() const {
      return Desc.begin() + getDescIdxBeginOfCertainLine<Width>();
    }
    template <uint32_t Width> Cells::iterator getDescBeginOfCertainLine() {
      return Desc.begin() + getDescIdxBeginOfCertainLine<Width>();
    }

    template <uint32_t Width>
    Cells::const_iterator getDescEndOfCertainLine() const {
      uint32_t constexpr NextLine = Width << 1u;
      return Desc.begin() + getDescIdxBeginOfCertainLine<NextLine>();
    }
    template <uint32_t Width> Cells::iterator getDescEndOfCertainLine() {
      uint32_t constexpr NextLine = Width << 1u;
      return Desc.begin() + getDescIdxBeginOfCertainLine<NextLine>();
    }

    template <uint32_t Width> Cells::iterator getFirstFreeCell() {
      return std::find(getDescBeginOfCertainLine<Width>(),
                       getDescEndOfCertainLine<Width>(), Cell::FREE);
    }

    uint32_t getMemoryOffset(uint32_t DescIdx) const {
      uint32_t const Shifted = DescIdx << (32u - I::NumOfLineWidths);
      uint32_t const RowIdx = std::countl_one(Shifted);
      uint32_t const InnerOffsetMask =
          (1 << (I::NumOfLineWidths - RowIdx)) - 1u;
      uint32_t const InnerOffset = InnerOffsetMask & DescIdx;
      return InnerOffset;
    }

    uint32_t getMemoryOffset(Cells::const_iterator It) const {
      uint32_t const DescIdx = std::distance(Desc.begin(), It);
      return getMemoryOffset(DescIdx);
    }

    uint32_t getMemoryOffset(Cells::iterator It) {
      uint32_t const DescIdx = std::distance(Desc.begin(), It);
      return getMemoryOffset(DescIdx);
    }

  public:
    CellDescription() : Desc{I::TotalLines, Cell::FREE} {
      std::cerr << "Allocating Description: " << Desc.size() << std::endl;
    }

    template <uint32_t Width> std::optional<uint32_t> retrieveFirstFree() {
      auto const Begin = getDescBeginOfCertainLine<Width>();
      auto const End = getDescEndOfCertainLine<Width>();
      auto const FindIt = std::find(Begin, End, Cell::FREE);
      if (FindIt == End)
        return {};
      *FindIt = Cell::OCCUPIED;
      return getMemoryOffset(FindIt);
    }

    std::ostream &print(std::ostream &os) const {
      uint32_t Inc = 0;
      uint32_t Latch = I::MinInMax;
      os << '|';
      for (auto const &C : Desc) {
        printCell(os, C);
        if (++Inc == Latch) {
          os << '|';
          Latch /= 2;
          Inc = 0;
        }
      }
      return os;
    }
  } CD;

  void *Buffer = nullptr;

  static void *allocMemory();

  uint32_t getDescIdx(Point *Pnt) const;

public:
  MemoryManager();

  template <uint32_t Width> Line<Width> createLine() {
    std::cerr << "Attempt to create Line<" << Width << ">." << std::endl;
    auto const IsOffset = CD.retrieveFirstFree<Width>();
    if (!IsOffset) {
      std::cerr << "No free cells for Line<" << Width << ">." << std::endl;
      return Line<Width>::createLine(nullptr);
    }
    // TODO Offset does not work.
    uint32_t const Offset = IsOffset.value();
    I::DummyLine *Start = static_cast<I::DummyLine *>(Buffer);
    Point *Ptr = reinterpret_cast<Point *>(Start + Offset);
    // CD.print(std::cerr) << std::endl;
    return Line<Width>::createLine(Ptr);
  }

  ~MemoryManager();
};

} // namespace nmgr
