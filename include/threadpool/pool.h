#pragma once

#include <atomic>
#include <memory>
#include <optional>
#include <queue>
#include <thread>

#include "threadpool/job.h"

namespace threadpool {
class ThreadPool {
 public:
  ThreadPool(size_t threads);
  ~ThreadPool();

  void Push(std::unique_ptr<Job> &&job);

 private:
  class JobQueue {
   private:
    std::queue<std::unique_ptr<Job>> data_;
    std::mutex m_;
    std::condition_variable cv_;

   public:
    inline ~JobQueue() { fprintf(stderr, "JobQueue done\n"); }

    size_t size();
    inline std::mutex &mutex() { return m_; }
    inline std::condition_variable &cv() { return cv_; }

    std::unique_ptr<Job> Pop();
    void Push(std::unique_ptr<Job> &&job);
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