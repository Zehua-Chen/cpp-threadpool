#include "threadpool/job_group.h"

namespace threadpool {
JobGroup::JobGroup() : count_(0) {}

void JobGroup::Enter() {
  std::unique_lock<std::mutex> lock{m_};
  count_ += 1;
  cv_.notify_all();
}

void JobGroup::Leave() {
  std::unique_lock<std::mutex> lock{m_};
  count_ -= 1;
  cv_.notify_all();
}

void JobGroup::Wait() {
  std::unique_lock<std::mutex> lock{m_};

  while (count_ != 0) {
    cv_.wait(lock);
  }
}
}  // namespace threadpool