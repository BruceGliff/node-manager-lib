#include "MemoryManager.h"

#include "nmgr/point.h"

#include <iostream>

using namespace nmgr;

MemoryManager::MemoryManager() : Buffer(allocMemory()) {}

CellDescription::Cell const &MemoryManager::getDescIdx(Point *Pnt) const {
  // This struct only need for address calculating.
  // The smallest line entity is the Line<2> and all calculating are in terms of
  // Line<2>.
  // TODO get rid of DummyLine.
  struct DummyLine {
    Point P[2];
  };
  DummyLine const *Begin = reinterpret_cast<DummyLine *>(Buffer);
  DummyLine const *Ptr = reinterpret_cast<DummyLine *>(Pnt);
  uint32_t const P = Ptr - Begin; // Number of Line<2>.

  uint32_t const RowIdx = P / MinInMax;
  uint32_t const Stide = 1 << RowIdx; // 2^RowIdx.
  uint32_t const OffsetInLine = (P % MinInMax) / Stide;

  uint32_t const LineCountOnes = (1u << NumOfLineWidths) - 1;
  uint32_t const Zeros = NumOfLineWidths - RowIdx;
  uint32_t const LinesBefore = LineCountOnes & Zeros;

  uint32_t const DescOffset = OffsetInLine + LinesBefore;

  return CD.at(DescOffset);
}

void *MemoryManager::allocMemory() {
  std::cerr << "Alloca:\n\tMaxLineWidth = " << MaxLineWidth
            << "\n\tPointsToAlloc = " << TotalPoints << '\n';
  void *Buffer = ::operator new(sizeof(Point) * TotalPoints);
  std::cerr << "\tBuffer size = " << sizeof(Point) * TotalPoints << "\n"
            << std::endl;
  return Buffer;
}

MemoryManager::~MemoryManager() { ::operator delete(Buffer); }