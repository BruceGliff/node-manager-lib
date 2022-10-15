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

// MemoryManager is a class which allocates memory for all Lines.
// The basic unit is Line<2> and MinLineWidth = 2. Main properties of allocation
// are:
// 1. It allocates linear memory once.
// 2. It parameterized with max Line width.
// 3. It allocates enouth memory for:
//      - One  Line<MaxLineWidth>
//      - Two  Lines<MaxLineWidth / 2>
//      - Four Lines<MaxLineWidth / 4>
//      - ...
//      - MaxLineWidth/2 Lines<2>
// 4. In total it allocates memory for (MaxLineWidth - 1) lines or for
// (MaxLineWidth / 2 * getPowOfTwo(MaxLineWidth)) Lines<2>. Memory can be
// represented as matrix where rows are correspondent to different line widths.
// Example for MaxWidth = 16. '.' means Point.
//   Line<2>:  |..|..|..|..|..|..|..|..|
//   Line<4>:  |.. ..|.. ..|.. ..|.. ..|
//   Line<8>:  |.. .. .. ..|.. .. .. ..|
//   Line<16>: |.. .. .. .. .. .. .. ..|
// PS: but memory is linear.
class MemoryManager final {

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
    static constexpr uint32_t MaxLineWidth = Width;
    // Number of different types of Line.
    static constexpr uint32_t NumOfLineWidths = getPowOfTwo<MaxLineWidth>();
    // Description size.
    static constexpr uint32_t TotalLines = MaxLineWidth - 1u;
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

  // CellDescription is a class which describes which lines are occupied and
  // which are free. It can be represented as an array of Cells. For MaxWidth =
  // 16: Cells: |........|....|..|.| where '.' means Cell or Line.
  class CellDescription final {
    enum class Cell : uint8_t { FREE = 0, OCCUPIED, UNDEF };
    static std::ostream &printCell(std::ostream &os, Cell const &C);

    using Cells = std::vector<Cell>;
    Cells Desc;

    // Returns the index which indicates the begining of the description of
    // sertain lines with the Width.
    template <uint32_t Width>
    inline uint32_t constexpr getDescIdxBeginOfCertainLine() const;

    // Returns the const iterator of Desc indicates the begining of the
    // description of sertain lines with the Width.
    template <uint32_t Width>
    inline Cells::const_iterator getDescBeginOfCertainLine() const;
    // Returns the iterator of Desc indicates the begining of the description of
    // sertain lines with the Width.
    template <uint32_t Width>
    inline Cells::iterator getDescBeginOfCertainLine();

    // Returns the const iterator of Desc indicates the ending of the
    // description of sertain lines with the Width. It is the same as the
    // begining of line with Width*2.
    template <uint32_t Width>
    Cells::const_iterator inline getDescEndOfCertainLine() const;
    // Returns the iterator of Desc indicates the ending of the description of
    // sertain lines with the Width. It is the same as the begining of line with
    // Width*2.
    template <uint32_t Width> inline Cells::iterator getDescEndOfCertainLine();

    // Returns the iterator of Desc to the first free cell for sertain line with
    // the Width. If there are no free cells, it will return end of the
    // description of the sertain line (or end of Description for
    // Line<MaxWidth>).
    template <uint32_t Width> inline Cells::iterator getFirstFreeCell();

    // Returns offset of the Line in MemoryManager which is defined by DescIdx.
    uint32_t getMemoryOffset(uint32_t DescIdx) const;
    // Returns offset of the Line in MemoryManager which is defined by It.
    uint32_t getMemoryOffset(Cells::const_iterator It) const;
    // Returns offset of the Line in MemoryManager which is defined by It.
    uint32_t getMemoryOffset(Cells::iterator It);

  public:
    CellDescription();

    // Returns offset for the Line<Width> which is not occupied and occupies
    // corresponsible cell. Returns None if there is no free space for line.
    template <uint32_t Width>
    std::optional<uint32_t> inline retrieveFirstFree();

    // Prints whole description.
    std::ostream &print(std::ostream &os) const;
  } CD;

  void *Buffer = nullptr;

  // Allocates raw memory by the information given by I.
  static void *allocMemory();

  // Returns position of the sertain line description.
  // TODO not tested. Believe it should be getDescIdx(Line L). Copy of the line
  // is possible because it contains only pointer.
  uint32_t getDescIdx(Point const *Pnt) const;
  // TODO this is public just to check everything is ok.
public:
  template <typename T>
    requires std::is_base_of_v<LineBase, T>
  uint32_t getDescIdx(T L) const {
    return getDescIdx(L.Pts);
  }

public:
  MemoryManager();

  // Returns free Line<Width>.
  // If there is no free space for line it will return Line with
  // pointer=nullptr.
  // TODO override bool operator?
  template <uint32_t Width> Line<Width> inline createLine();

  ~MemoryManager();
};

} // namespace nmgr

#include "MemoryManager.hpp"
