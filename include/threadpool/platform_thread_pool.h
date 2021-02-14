#pragma once

#include "threadpool/thread_pool_shared.h"

namespace threadpool {
class platform_thread_pool : public thread_pool {
 public:
  platform_thread_pool();
  void push(std::unique_ptr<job> &&job) override;
};
}  // namespace threadpool
