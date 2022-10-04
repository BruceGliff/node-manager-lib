#include "MemoryManager.h"

#include "nmgr/point.h"

#include <bit>
#include <cassert>
#include <iostream>

using namespace nmgr;

uint32_t CellDescription::getLineIdx(uint32_t Pos) const {
  uint32_t const Shifted = Pos << (32 - MemoryManager::NumOfLineWidths);
  uint32_t const RowIdx = std::countl_one(Shifted);
  uint32_t const InnerOffsetMask =
      (1 << (MemoryManager::NumOfLineWidths - RowIdx)) - 1;
  uint32_t const InnerOffset = InnerOffsetMask & Pos;
  return InnerOffset;
}

uint32_t CellDescription::occupyLine(uint32_t Pos) {
  assert(Desc[Pos] == Cell::FREE && "Occupying busy line.");
  Desc[Pos] = Cell::OCCUPIED;
  return getLineIdx(Pos);
}

MemoryManager::MemoryManager() : Buffer(allocMemory()) {}

CellDescription::Cell const &MemoryManager::getDescIdx(Point *Pnt) const {
  DummyLine const *Begin = static_cast<DummyLine *>(Buffer);
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