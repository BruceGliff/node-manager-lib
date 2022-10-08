#include "nmgr/interface.h"

#include "MemoryManager.h"

void nmgr::createMemory() {
  MemoryManager M;
  // M.createLine<1u>();
  M.createLine<2u>();
  M.createLine<4u>();
  M.createLine<4u>();
  M.createLine<4u>();
  M.createLine<8u>();
  M.createLine<16u>();
  M.createLine<16u>();
  M.createLine<16u>();
  M.createLine<32u>();
  // M.createLine<128u>();
  // M.createLine<1u << 28>();
  // M.createLine<1u << 31>();
}