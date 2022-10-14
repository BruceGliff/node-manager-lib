#include "nmgr/interface.h"

#include "MemoryManager.h"

void nmgr::createMemory() {
  MemoryManager M;
  // M.createLine<1u>();
  auto L2 = M.createLine<2u>();
  std::cout << "Back: " << M.getDescIdx(L2) << std::endl;
  auto L2_1 = M.createLine<2u>();
  std::cout << "Back: " << M.getDescIdx(L2_1) << std::endl;

  // auto L41 = M.createLine<8u>();
  // auto L42 = M.createLine<8u>();
  // auto L43 = M.createLine<8u>();
  // auto L44 = M.createLine<8u>();

  auto L4 = M.createLine<4u>();
  std::cout << "Back: " << M.getDescIdx(L4) << std::endl;
  auto L4_1 = M.createLine<4u>();
  std::cout << "Back: " << M.getDescIdx(L4_1) << std::endl;
  auto L8 = M.createLine<8u>();
  std::cout << "Back: " << M.getDescIdx(L8) << std::endl;
  auto L4_2 = M.createLine<4u>();
  std::cout << "Back: " << M.getDescIdx(L4_2) << std::endl;
  auto L16 = M.createLine<16u>();
  std::cout << "Back: " << M.getDescIdx(L16) << std::endl;
  auto L16_1 = M.createLine<16u>();
  std::cout << "Back: " << M.getDescIdx(L16_1) << std::endl;
  auto L16_2 = M.createLine<16u>();
  std::cout << "Back: " << M.getDescIdx(L16_2) << std::endl;
  auto L32 = M.createLine<32u>();
  std::cout << "Back: " << M.getDescIdx(L32) << std::endl;
  // M.createLine<128u>();
  // M.createLine<1u << 28>();
  // M.createLine<1u << 31>();
}