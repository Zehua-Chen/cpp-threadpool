#include "threadpool/pool.h"

namespace threadpool {
ThreadPool::Worker::Worker(Worker &&other)
    : pool{other.pool}, thread{std::move(other.thread)} {}

ThreadPool::Worker::Worker(ThreadPool *pool) : pool{pool} {
  std::thread actual_thread([this]() {
    while (this->pool->running_) {
      std::unique_lock<std::mutex> lock{this->pool->m_};

      while (this->pool->jobs_.size() < 1 && this->pool->running_) {
        this->pool->cv_.wait(lock);

        if (!this->pool->running_) {
          return;
        }
      }

      Job *job = this->pool->jobs_.front();
      this->pool->jobs_.pop();
      this->pool->cv_.notify_all();

      lock.unlock();
      job->Execute();
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
  running_ = false;
  cv_.notify_all();
}

void ThreadPool::Push(Job *job) {
  std::unique_lock<std::mutex> lock;
  jobs_.push(job);
  cv_.notify_all();
}

void ThreadPool::Wait() {
  std::unique_lock<std::mutex> lock{m_};

  while (jobs_.size() != 0) {
    cv_.wait(lock);
  }
}
}  // namespace threadpool
