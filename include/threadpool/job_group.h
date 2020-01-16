#pragma once

#include <condition_variable>
#include <mutex>

namespace threadpool {
/**
 * @brief A way to control a group of jobs
 */
class JobGroup {
 public:
  /**
   * @brief Create a new job group
   */
  JobGroup();

  /**
   * @brief Notify the job group that a job has entered the job group
   */
  void Enter();

  /**
   * @brief Notify the job group that a job has left the job group
   */
  void Leave();

  /**
   * @brief Block until all jobs that have entered a job group has left the job
   * group
   */
  void Wait();

 private:
  int count_;
  std::mutex m_;
  std::condition_variable cv_;
};
}  // namespace threadpool