#pragma once

#include <memory>

namespace threadpool {
/**
 * @brief An job that can be submitted to the threadpool to be executed
 */
class job {
 public:
  enum class priority {
    low,
    medium,
    high,
  };

  priority priority;

  virtual ~job();

  /**
   * @brief Called when the job is to be executed
   */
  virtual void execute() = 0;
};

/**
 * @brief Create a job using an functor
 *
 * @param func a functor
 * @returns a job that execute a functor
 */
template <typename Func>
std::unique_ptr<job> create_job(Func func);
}  // namespace threadpool

namespace threadpool {
template <typename Func>
std::unique_ptr<job> create_job(Func func) {
  struct func_job : job {
    Func func;

    func_job(Func func) : func(func) {}

    void execute() override { func(); }
  };

  std::unique_ptr<job> job{new func_job(func)};

  return job;
}
}  // namespace threadpool
