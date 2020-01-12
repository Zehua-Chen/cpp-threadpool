#pragma once

#include <atomic>
#include <memory>
#include <queue>
#include <thread>

#include "threadpool/job.h"

namespace threadpool {
class ThreadPool {
 public:
  ThreadPool(size_t threads);
  ~ThreadPool();

  void Push(std::unique_ptr<Job> &&job);
  void Wait();

 private:
  std::queue<std::unique_ptr<Job>> jobs_;
  bool running_ = true;
  std::mutex m_;
  std::condition_variable cv_;

  struct Worker {
    ThreadPool *pool;
    std::thread thread;
    std::atomic<bool> busy;

    Worker(Worker &&other);
    Worker(ThreadPool *pool);

    ~Worker();
  };

  std::vector<Worker> workers_;
};
}  // namespace threadpool