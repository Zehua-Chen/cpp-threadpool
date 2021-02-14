#pragma once

namespace threadpool {
/**
 * Standard thread_pool interface
 */
class thread_pool {
  /**
   * Push a new job
   * @param job the job to run, the ownership of the job will be transfered
   * to the thread pool
   */
  virtual void push(std::unique_ptr<job> &&job) = 0;
};
}  // namespace threadpool
