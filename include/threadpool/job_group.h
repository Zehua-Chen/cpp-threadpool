#pragma once

#include <condition_variable>
#include <mutex>

namespace threadpool {
class JobGroup {
 public:
  JobGroup();

  void Enter();
  void Leave();

  void Wait();

 private:
  int count_;
  std::mutex m_;
  std::condition_variable cv_;
};
}  // namespace threadpool