#include "nmgr/lib-interface.h"

#include "MemoryManager.h"
#include "nmgr/line.hpp"

using namespace nmgr;

namespace LIB {

Line<2> createDefaultLine() {
  return MemoryManager::getInstance().createLine<2>();
}

} // namespace LIB
