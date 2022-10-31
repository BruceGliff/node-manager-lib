#include "MemoryManager.h"
#include "collision.h"

#define INSTANCE(Class)                                                        \
  template nmgr::singleton<Class>::InstanceTy nmgr::singleton<Class>::Instance;

INSTANCE(nmgr::MemoryManager);
INSTANCE(nmgr::collision::CollManager);
