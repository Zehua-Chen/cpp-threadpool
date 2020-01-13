#pragma once

#include <atomic>
#include <memory>
#include <optional>
#include <queue>
#include <thread>

#include "threadpool/job.h"

namespace threadpool {
/*
 * @brief A threadpool object
 */
class ThreadPool {
 public:
  /**
   * @brief create a thread pool
   * Create a thread pool
   *
   * @param threads how many threads to use
   */
  ThreadPool(size_t threads);
  ~ThreadPool();

  /**
   * @brief Push a new job onto the thread pool
   * Push a new job onto the thread pool
   *
   * @param job a job to be run. Note that the ownership of the job will be
   * trasfered over to the thread pool. The job can be null
   */
  void Push(std::unique_ptr<Job> &&job);

 private:
  /**
   * @brief A queue of jobs
   *
   * A thread safe implementation of a queue of jobs
   */
  class JobQueue {
   private:
    std::queue<std::unique_ptr<Job>> data_;
    std::mutex m_;
    std::condition_variable cv_;

   public:
    /**
     * Get the size of the queue
     *
     * @return the size of the queue
     */
    inline size_t size() { return data_.size(); }

    /**
     * Get a job and return it from the queue
     *
     * @returns return a job if there is one; if there are no more jobs,
     * two conditions might occur. 1. a null is returned if `Close()` is called.
     * 2. block until either `Close()` is called, or a new job is pushed
     */
    std::unique_ptr<Job> Pop();

    /**
     * Get a job and return it from the queue
     *
     * @param push a new job onto the queue, with the ownership transferred to
     * the queue
     */
    void Push(std::unique_ptr<Job> &&job);

    /**
     * Close the queue, would cause `Pop()` to return null if there are no more
     * jobs available.
     */
    void Close();
  };

  class Worker {
   private:
    ThreadPool *pool_;
    std::thread thread_;

   public:
    Worker(Worker &&other);
    Worker(ThreadPool *pool);

    ~Worker();
  };

  std::atomic<size_t> threads_;
  bool open_;

  // Note: queue_ must be put before workers_
  // workers_ rely on the existance of queue_, therefore, workers_ must be
  // put below queue_ so that it is destructed first
  JobQueue queue_;
  std::vector<Worker> workers_;
};
}  // namespace threadpool