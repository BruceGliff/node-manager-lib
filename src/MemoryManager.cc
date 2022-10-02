#include "MemoryManager.h"

#include "nmgr/point.h"

#include <iostream>

using namespace nmgr;

MemoryManager::MemoryManager() : Buffer(allocMemory()), CD(calcSize()) {}

void *MemoryManager::allocMemory() {
  unsigned constexpr MaxLineWidth = 1 << 16;
  unsigned constexpr PointsToAlloc = MaxLineWidth * 16;

  std::cerr << "Alloca:\n\tMaxLineWidth = " << MaxLineWidth
            << "\n\tPointsToAlloc = " << PointsToAlloc << '\n';
  void *Buffer = ::operator new(sizeof(Point) * PointsToAlloc);
  std::cerr << "\tBuffer size = " << sizeof(Point) * PointsToAlloc << "\n"
            << std::endl;
  return Buffer;
}

unsigned MemoryManager::calcSize() {
  return /*lines*/ 16 * /*MaxLineWidth*/ (1 << 16) /
         /*because number of line in row is decreasing*/ 2;
}

MemoryManager::~MemoryManager() { ::operator delete(Buffer); }