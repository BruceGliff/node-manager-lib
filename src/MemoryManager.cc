#include "MemoryManager.h"

#include "nmgr/point.h"

#include <bit>
#include <cassert>
#include <iostream>

using namespace nmgr;

MemoryManager::MemoryManager() : Buffer(allocMemory()) {}

uint32_t MemoryManager::getDescIdx(Point *Pnt) const {
  using DummyLine = I::DummyLine;
  DummyLine const *Begin = static_cast<DummyLine *>(Buffer);
  DummyLine const *Ptr = reinterpret_cast<DummyLine *>(Pnt);
  uint32_t const P = Ptr - Begin; // Number of Line<2>.

  uint32_t const RowIdx = P / I::MinInMax;
  uint32_t const Stide = 1 << RowIdx; // 2^RowIdx.
  uint32_t const OffsetInLine = (P % I::MinInMax) / Stide;

  uint32_t const LineCountOnes = (1u << I::NumOfLineWidths) - 1;
  uint32_t const Zeros = I::NumOfLineWidths - RowIdx;
  uint32_t const LinesBefore = LineCountOnes & Zeros;

  uint32_t const DescOffset = OffsetInLine + LinesBefore;
  return DescOffset;
}

void *MemoryManager::allocMemory() {
  std::cerr << "Alloca:\n\tMaxLineWidth = " << I::MaxLineWidth
            << "\n\tPointsToAlloc = " << I::TotalPoints << '\n';
  void *Buffer = ::operator new(sizeof(Point) * I::TotalPoints);
  std::cerr << "\tBuffer size = " << sizeof(Point) * I::TotalPoints << "\n"
            << std::endl;
  return Buffer;
}

MemoryManager::~MemoryManager() { ::operator delete(Buffer); }