#pragma once

namespace nmgr {

class MemoryManager {

  void *Buffer = nullptr;

  static void *allocMemory();

public:
  MemoryManager();

  ~MemoryManager();
};

} // namespace nmgr