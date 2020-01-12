#include "threadpool/pool.h"

#include <algorithm>

namespace threadpool {
ThreadPool::Worker::Worker(Worker &&other)
    : pool{other.pool}, thread{std::move(other.thread)} {}

ThreadPool::Worker::Worker(ThreadPool *pool) : pool{pool}, busy{false} {
  std::thread actual_thread([this]() {
    while (this->pool->running_) {
      std::unique_lock<std::mutex> lock{this->pool->m_};

      while (this->pool->jobs_.size() < 1 && this->pool->running_) {
        this->pool->cv_.wait(lock);

        if (!this->pool->running_) {
          return;
        }
      }

      this->busy.store(true);

      std::unique_ptr<Job> job = std::move(this->pool->jobs_.front());
      this->pool->jobs_.pop();
      this->pool->cv_.notify_all();

      lock.unlock();
      job->Execute();

      this->busy.store(false);
    }
  });

  std::swap(thread, actual_thread);
}

ThreadPool::Worker::~Worker() { thread.join(); }

ThreadPool::ThreadPool(size_t threads) {
  workers_.reserve(threads);

  for (size_t i = 0; i < threads; ++i) {
    workers_.emplace_back(this);
  }
}

ThreadPool::~ThreadPool() {
  std::unique_lock<std::mutex> lock{m_};
  running_ = false;
  cv_.notify_all();
}

void ThreadPool::Push(std::unique_ptr<Job> &&job) {
  std::unique_lock<std::mutex> lock{m_};
  jobs_.push(std::move(job));
  cv_.notify_all();
}

void ThreadPool::Wait() {
  std::unique_lock<std::mutex> lock{m_};
  const auto predicate = [](const Worker &worker) {
    return worker.busy.load();
  };

  while (std::any_of(workers_.begin(), workers_.end(), predicate)) {
    cv_.wait(lock);
  }
}
}  // namespace threadpool
