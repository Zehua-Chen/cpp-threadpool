#include "threadpool/job_group.h"

namespace threadpool {
job_group::job_group() : count_(0) {}

void job_group::enter() {
  std::unique_lock<std::mutex> lock{m_};
  count_ += 1;
  cv_.notify_all();
}

void job_group::leave() {
  std::unique_lock<std::mutex> lock{m_};
  count_ -= 1;
  cv_.notify_all();
}

void job_group::wait() {
  std::unique_lock<std::mutex> lock{m_};

  while (count_ != 0) {
    cv_.wait(lock);
  }
}
}  // namespace threadpool
