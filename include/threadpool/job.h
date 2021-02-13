#pragma once

#include <memory>

namespace threadpool {
/**
 * @brief An job that can be submitted to the threadpool to be executed
 */
class job {
 public:
  /**
   * @brief Called when the job is to be executed
   */
  virtual void execute() = 0;
  virtual ~job();
};

/**
 * @brief Create a job using an functor
 *
 * @param func a functor
 * @returns a job that execute a functor
 */
template <typename FuncT>
std::unique_ptr<job> create_job(FuncT func);
}  // namespace threadpool

namespace threadpool {
template <typename FuncT>
std::unique_ptr<job> create_job(FuncT func) {
  struct func_job : job {
    FuncT func;

    func_job(FuncT func) : func(func) {}

    void execute() override { func(); }
  };

  std::unique_ptr<job> job{new func_job(func)};

  return job;
}
}  // namespace threadpool
