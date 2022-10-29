#include "MemoryManager.h"

#define INSTANCE(Class)                                                        \
  template nmgr::singleton<Class>::InstanceTy nmgr::singleton<Class>::Instance;

INSTANCE(nmgr::MemoryManager);
