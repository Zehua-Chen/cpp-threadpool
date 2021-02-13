#include "threadpool/pool.h"

#include <algorithm>

using std::unique_ptr;

namespace threadpool {
unique_ptr<job> thread_pool::job_queue::pop() {
  std::unique_lock<std::mutex> lock{m_};

  if (data_.size() == 0) {
    return unique_ptr<job>{};
  }

  while (data_.size() < 1) {
    cv_.wait(lock);

    if (data_.size() == 0) {
      return unique_ptr<job>{};
    }
  }

  unique_ptr<job> job = std::move(data_.front());
  data_.pop();

  cv_.notify_one();

  return job;
}

void thread_pool::job_queue::push(unique_ptr<job> &&job) {
  std::unique_lock<std::mutex> lock{m_};

  data_.push(std::move(job));
  cv_.notify_one();
}

void thread_pool::job_queue::close() { cv_.notify_all(); }

thread_pool::worker::worker(worker &&other)
    : pool_{other.pool_}, thread_{std::move(other.thread_)} {
  other.pool_ = nullptr;
}

thread_pool::worker::worker(thread_pool *pool) : pool_{pool} {
  std::thread actual_thread([this]() {
    while (this->pool_->open_) {
      unique_ptr<job> job = this->pool_->queue_.pop();

      // Close the worker without running the job
      if (!this->pool_->open_) {
        break;
      }

      if (job) {
        job->execute();
      }
    }
  });

  std::swap(thread_, actual_thread);
}

thread_pool::worker::~worker() { thread_.join(); }

thread_pool::thread_pool(size_t threads) : threads_(threads), open_(true) {
  workers_.reserve(threads);

  for (size_t i = 0; i < threads; ++i) {
    workers_.emplace_back(this);
  }
}

thread_pool::~thread_pool() {
  open_ = false;
  queue_.close();
}

void thread_pool::push(unique_ptr<job> &&job) { queue_.push(std::move(job)); }

thread_pool *thread_pool::shared() {
  if (!default_pool_) {
    unsigned int concurrency = std::thread::hardware_concurrency();

    default_pool_ = unique_ptr<thread_pool>{new thread_pool{concurrency - 1}};
  }

  return default_pool_.get();
}

unique_ptr<thread_pool> thread_pool::default_pool_{};
}  // namespace threadpool
