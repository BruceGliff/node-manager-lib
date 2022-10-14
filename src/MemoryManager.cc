#include "MemoryManager.h"

#include "nmgr/point.h"

#include <bit>
#include <cassert>
#include <iostream>

using namespace nmgr;

std::ostream &MemoryManager::CellDescription::printCell(std::ostream &os,
                                                        Cell const &C) {
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

uint32_t
MemoryManager::CellDescription::getMemoryOffset(uint32_t DescIdx) const {
  uint32_t const Shifted = DescIdx << (32u - I::NumOfLineWidths);
  uint32_t const RowIdx = std::countl_one(Shifted);
  uint32_t const InnerOffsetMask = (1u << (I::NumOfLineWidths - RowIdx)) - 1u;
  uint32_t const InnerOffsetWholeLine = InnerOffsetMask & DescIdx;
  uint32_t const InnerOffsetMinLine = InnerOffsetWholeLine << RowIdx;
  uint32_t const MemoryOffset = RowIdx * I::MinInMax + InnerOffsetMinLine;
  return MemoryOffset;
}

uint32_t MemoryManager::CellDescription::getMemoryOffset(
    Cells::const_iterator It) const {
  uint32_t const DescIdx = std::distance(Desc.begin(), It);
  return getMemoryOffset(DescIdx);
}

uint32_t MemoryManager::CellDescription::getMemoryOffset(Cells::iterator It) {
  uint32_t const DescIdx = std::distance(Desc.begin(), It);
  std::cerr << "Ref: " << DescIdx << std::endl;
  return getMemoryOffset(DescIdx);
}

MemoryManager::CellDescription::CellDescription()
    : Desc{I::TotalLines, Cell::FREE} {
  std::cerr << "Allocating Description: " << Desc.size() << std::endl;
}

std::ostream &MemoryManager::CellDescription::print(std::ostream &os) const {
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

MemoryManager::MemoryManager() : Buffer(allocMemory()) {}

uint32_t MemoryManager::getDescIdx(Point const *Pnt) const {
  if (!Pnt)
    return 0u;
  using DummyLine = I::DummyLine;
  DummyLine const *Begin = static_cast<DummyLine const *>(Buffer);
  DummyLine const *Ptr = reinterpret_cast<DummyLine const *>(Pnt);
  uint32_t const P = Ptr - Begin; // Number of Line<2>.

  uint32_t const RowIdx = P / I::MinInMax;
  uint32_t const Stride = 1 << RowIdx; // 2^RowIdx.
  uint32_t const OffsetInLine = (P % I::MinInMax) / Stride;

  uint32_t constexpr LineCountOnes = (1u << I::NumOfLineWidths) - 1u;
  uint32_t const Zeros = I::NumOfLineWidths - RowIdx;
  uint32_t const LineZeros = ~((1u << Zeros) - 1u);
  uint32_t const LinesBefore = LineCountOnes & LineZeros;

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