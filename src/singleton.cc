#include "MemoryManager.h"

template <>
nmgr::singleton<nmgr::MemoryManager>::InstanceTy
    nmgr::singleton<nmgr::MemoryManager>::Instance{nullptr};
