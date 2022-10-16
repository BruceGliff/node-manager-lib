#pragma once

#include <memory>

namespace nmgr {

template <typename Parent> class singleton {
public:
  using InstanceTy = std::unique_ptr<Parent>;

private:
  static InstanceTy Instance;

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

} // namespace nmgr
