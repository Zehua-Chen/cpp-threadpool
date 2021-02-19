#include "threadpool/platform_thread_pool.h"

#include <cstdio>

using std::unique_ptr;
using threadpool::job;

#ifdef __APPLE__

struct job_context {
  std::unique_ptr<job> job;
};

static void run_job(void *ptr) {
  if (ptr == nullptr) {
    return;
  }

  const auto *context = reinterpret_cast<job_context *>(ptr);
  context->job->execute();

  delete context;
}

#endif

namespace threadpool {
platform_thread_pool::platform_thread_pool(
    const platform_priorities &priorities)
    : priorities(priorities) {

#ifdef __APPLE__
  high_priority_queue_ =
      dispatch_get_global_queue(priorities.high_priority_qos, 0);

  medium_priority_queue_ =
      dispatch_get_global_queue(priorities.medium_priority_qos, 0);

  low_priority_queue_ =
      dispatch_get_global_queue(priorities.low_priority_qos, 0);
#endif
}

void platform_thread_pool::push(unique_ptr<job> &&job) {
#ifdef __APPLE__
  job_context *context = new job_context{};
  context->job = std::move(job);

  const auto get_queue = [this](priority priority) {
    switch (priority) {
      case priority::high:
        return this->high_priority_queue_;
      case priority::medium:
        return this->medium_priority_queue_;
      case priority::low:
        return this->low_priority_queue_;
    }
  };

  dispatch_queue_t queue = get_queue(job->priority);
  dispatch_async_f(queue, context, run_job);

#endif
}
}  // namespace threadpool
