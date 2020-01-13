#include "threadpool/pool.h"

#include <algorithm>

namespace threadpool {
std::unique_ptr<Job> ThreadPool::JobQueue::Pop() {
  std::unique_lock<std::mutex> lock{m_};

  if (data_.size() == 0) {
    return std::unique_ptr<Job>{};
  }

  while (data_.size() < 1) {
    cv_.wait(lock);

    if (data_.size() == 0) {
      return std::unique_ptr<Job>{};
    }
  }

  std::unique_ptr<Job> job = std::move(data_.front());
  data_.pop();

  cv_.notify_one();

  return job;
}

void ThreadPool::JobQueue::Push(std::unique_ptr<Job> &&job) {
  std::unique_lock<std::mutex> lock{m_};

  data_.push(std::move(job));
  cv_.notify_one();
}

void ThreadPool::JobQueue::Close() {
  cv_.notify_all();
}

ThreadPool::Worker::Worker(Worker &&other)
    : pool_{other.pool_}, thread_{std::move(other.thread_)} {
  other.pool_ = nullptr;
}

ThreadPool::Worker::Worker(ThreadPool *pool) : pool_{pool} {
  std::thread actual_thread([this]() {
    while (this->pool_->open_) {
      std::unique_ptr<Job> job = this->pool_->queue_.Pop();

      // Close the worker without running the job
      if (!this->pool_->open_) {
        break;
      }

      if (job) {
        job->Execute();
      }
    }
  });

  std::swap(thread_, actual_thread);
}

ThreadPool::Worker::~Worker() { thread_.join(); }

ThreadPool::ThreadPool(size_t threads) : threads_(threads) {
  workers_.reserve(threads);

  for (size_t i = 0; i < threads; ++i) {
    workers_.emplace_back(this);
  }
}

ThreadPool::~ThreadPool() {
  open_ = false;
  queue_.Close();
}

void ThreadPool::Push(std::unique_ptr<Job> &&job) {
  queue_.Push(std::move(job));
}
}  // namespace threadpool
