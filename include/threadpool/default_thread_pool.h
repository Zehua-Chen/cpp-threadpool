#pragma once

#include <atomic>
#include <memory>
#include <optional>
#include <queue>
#include <thread>

#include "threadpool/job.h"
#include "threadpool/thread_pool_shared.h"

namespace threadpool {
/**
 * @brief A default object
 */
class default_thread_pool : public thread_pool {
 public:
  /**
   * @brief Create a thread pool
   *
   * @param threads how many threads to use
   */
  default_thread_pool(size_t threads);
  ~default_thread_pool();

  /**
   * @brief Push a new job onto the thread pool
   *
   * @param job a job to be run. Note that the ownership of the job will be
   * trasfered over to the thread pool. The job can be null
   */
  void push(std::unique_ptr<job> &&job) override;

  /**
   * @brief Get the default thread pool
   *
   * @returns Returns a thread pool with # of threads =
   * `std::thread::hardware_concurrency() - 1`
   */
  static default_thread_pool *shared();

 private:
  /**
   * @brief A thread safe implementation of a queue of jobs
   */
  class job_queue {
   private:
    std::queue<std::unique_ptr<job>> data_;
    std::mutex m_;
    std::condition_variable cv_;

   public:
    /**
     * @brief Get the size of the queue
     *
     * @return the size of the queue
     */
    inline size_t size() { return data_.size(); }

    /**
     * @brief Get a job and return it from the queue
     *
     * @returns return a job if there is one; if there are no more jobs,
     * two conditions might occur. 1. a null is returned if `Close()` is called.
     * 2. block until either `Close()` is called, or a new job is pushed
     */
    std::unique_ptr<job> pop();

    /**
     * @brief Get a job and return it from the queue
     *
     * @param push a new job onto the queue, with the ownership transferred to
     * the queue
     */
    void push(std::unique_ptr<job> &&job);

    /**
     * @brief Close the queue, would cause `Pop()` to return null if there
     * are no more jobs available.
     */
    void close();
  };

  class worker {
   private:
    default_thread_pool *pool_;
    std::thread thread_;

   public:
    worker(worker &&other);
    worker(default_thread_pool *pool);

    ~worker();
  };

  std::atomic<size_t> threads_;
  std::atomic<bool> open_;

  // Note: queue_ must be put before workers_
  // workers_ rely on the existance of queue_, therefore, workers_ must be
  // put below queue_ so that it is destructed first
  job_queue queue_;
  std::vector<worker> workers_;

  static std::unique_ptr<default_thread_pool> default_pool_;
};
}  // namespace threadpool
