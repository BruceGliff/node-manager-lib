#pragma once

#include <memory>

namespace nmgr {

class Memorymanager;

template <typename Parent> class singleton {
public:
  using InstanceTy = std::unique_ptr<Parent>;

private:
  static inline InstanceTy Instance{nullptr};

protected:
  singleton() {}

public:
  static Parent &getInstance() {
    Parent *Ptr = Instance.get();
    if (!Ptr)
      Instance = std::unique_ptr<Parent>(new Parent);
    return *Instance.get();
  }
};

extern template singleton<MemoryManager>::InstanceTy
    singleton<MemoryManager>::Instance;

} // namespace nmgr
