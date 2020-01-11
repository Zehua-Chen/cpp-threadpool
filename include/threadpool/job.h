#pragma once

#include <memory>

namespace threadpool {
class Job {
 public:
  virtual void Execute() = 0;
  virtual ~Job();
};

template <typename FuncT>
std::unique_ptr<Job> Task(FuncT func);
}  // namespace threadpool

namespace threadpool {
template <typename FuncT>
std::unique_ptr<Job> Task(FuncT func) {
  struct FuncJob : Job {
    FuncT func;

    FuncJob(FuncT func) : func(func) {}

    void Execute() override { func(); }
  };

  std::unique_ptr<Job> job{new FuncJob(func)};

  return job;
}
}  // namespace threadpool