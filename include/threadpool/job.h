#pragma once

#include <memory>

namespace threadpool {
/**
 * @brief Priority of jobs
 */
enum class priority {
  low,
  medium,
  high,
};

/**
 * @brief An job that can be submitted to the threadpool to be executed
 */
class job {
 public:
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

/**
 * @brief Create a job using a functor and a priroity
 * @param priority priority of the job
 * @param func a functor
 * @returns a job with priority
 */
template <typename Func>
std::unique_ptr<job> create_job(priority priority, Func func);
}  // namespace threadpool

namespace threadpool {
template <typename Func>
std::unique_ptr<job> create_job(Func func) {
  /**
   * Job for lambdas
   */
  struct lambda_job : job {
    Func func;

    lambda_job(Func func) : func(func) {}

    void execute() override { func(); }
  };

  std::unique_ptr<job> job{new lambda_job(func)};

  return job;
}

template <typename Func>
std::unique_ptr<job> create_job(priority priority, Func func) {
  /**
   * Job for lambdas
   */
  struct lambda_job : job {
    Func func;

    lambda_job(Func func) : func(func) {}

    void execute() override { func(); }
  };

  std::unique_ptr<job> job{new lambda_job(func)};
  job->priority = priority;

  return job;
}
}  // namespace threadpool
