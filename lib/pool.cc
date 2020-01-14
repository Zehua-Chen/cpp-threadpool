#include "threadpool/pool.h"

#include <algorithm>

using std::unique_ptr;

namespace threadpool {
unique_ptr<Job> ThreadPool::JobQueue::Pop() {
  std::unique_lock<std::mutex> lock{m_};

  if (data_.size() == 0) {
    return unique_ptr<Job>{};
  }

  while (data_.size() < 1) {
    cv_.wait(lock);

    if (data_.size() == 0) {
      return unique_ptr<Job>{};
    }
  }

  unique_ptr<Job> job = std::move(data_.front());
  data_.pop();

  cv_.notify_one();

  return job;
}

void ThreadPool::JobQueue::Push(unique_ptr<Job> &&job) {
  std::unique_lock<std::mutex> lock{m_};

  data_.push(std::move(job));
  cv_.notify_one();
}

void ThreadPool::JobQueue::Close() { cv_.notify_all(); }

ThreadPool::Worker::Worker(Worker &&other)
    : pool_{other.pool_}, thread_{std::move(other.thread_)} {
  other.pool_ = nullptr;
}

ThreadPool::Worker::Worker(ThreadPool *pool) : pool_{pool} {
  std::thread actual_thread([this]() {
    while (this->pool_->open_) {
      unique_ptr<Job> job = this->pool_->queue_.Pop();

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

ThreadPool::ThreadPool(size_t threads) : threads_(threads), open_(true) {
  workers_.reserve(threads);

  for (size_t i = 0; i < threads; ++i) {
    workers_.emplace_back(this);
  }
}

ThreadPool::~ThreadPool() {
  open_ = false;
  queue_.Close();
}

void ThreadPool::Push(unique_ptr<Job> &&job) { queue_.Push(std::move(job)); }

ThreadPool *ThreadPool::Default() {
  if (!default_pool_) {
    unsigned int concurrency = std::thread::hardware_concurrency();

    default_pool_ = unique_ptr<ThreadPool>{new ThreadPool{concurrency - 1}};
  }

  return default_pool_.get();
}

unique_ptr<ThreadPool> ThreadPool::default_pool_{};
}  // namespace threadpool
