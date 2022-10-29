#pragma once

#include "requirements.h"
// This file is proxy communicator between logic in include and logic in lib.

namespace nmgr {

// Forward declarations.
template <uint32_t Width>
  requires LegalLine<Width>
class Line;

namespace LIB {
// Creates Line<2> with MemoryManager.
Line<2> createDefaultLine();

} // namespace LIB

} // namespace nmgr
