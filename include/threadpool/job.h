#pragma once

#include <memory>

namespace threadpool {
/**
 * @brief An job that can be submitted to the threadpool to be executed
 */
class Job {
 public:
  /**
   * @brief Called when the job is to be executed
   */
  virtual void Execute() = 0;
  virtual ~Job();
};

/**
 * @brief Create a job using an functor
 *
 * @param func a functor
 * @returns a job that execute a functor
 */
template <typename FuncT>
std::unique_ptr<Job> CreateJob(FuncT func);
}  // namespace threadpool

namespace threadpool {
template <typename FuncT>
std::unique_ptr<Job> CreateJob(FuncT func) {
  struct FuncJob : Job {
    FuncT func;

    FuncJob(FuncT func) : func(func) {}

    void Execute() override { func(); }
  };

  std::unique_ptr<Job> job{new FuncJob(func)};

  return job;
}
}  // namespace threadpool