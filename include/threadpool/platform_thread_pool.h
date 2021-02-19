#pragma once

#include "threadpool/thread_pool_shared.h"

#ifdef __APPLE__
#include <dispatch/dispatch.h>
#endif

namespace threadpool {
struct platform_priorities {
#ifdef __APPLE__
  intptr_t high_priority_qos = QOS_CLASS_USER_INTERACTIVE;
  intptr_t medium_priority_qos = QOS_CLASS_DEFAULT;
  intptr_t low_priority_qos = QOS_CLASS_UTILITY;
#endif
};

class platform_thread_pool : public thread_pool {
 public:
  platform_thread_pool(
      const platform_priorities &priorities = platform_priorities{});

  void push(std::unique_ptr<job> &&job) override;

  platform_priorities priorities;

 private:
#ifdef __APPLE__
  dispatch_queue_t high_priority_queue_;
  dispatch_queue_t medium_priority_queue_;
  dispatch_queue_t low_priority_queue_;
#endif
};
}  // namespace threadpool
