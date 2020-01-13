#include "threadpool/pool.h"

#include <algorithm>

namespace threadpool {
size_t ThreadPool::JobQueue::size() { return data_.size(); }

std::unique_ptr<Job> ThreadPool::JobQueue::Pop() {
  std::unique_lock<std::mutex> lock{m_};

  while (data_.size() < 1) {
    cv_.wait(lock);
  }

  std::unique_ptr<Job> job = std::move(data_.front());
  data_.pop();

  cv_.notify_all();

  return job;
}

void ThreadPool::JobQueue::Push(std::unique_ptr<Job> &&job) {
  std::unique_lock<std::mutex> lock{m_};

  data_.push(std::move(job));
  cv_.notify_all();
}

ThreadPool::Worker::Worker(Worker &&other)
    : pool_{other.pool_}, thread_{std::move(other.thread_)} {
  other.pool_ = nullptr;
}

ThreadPool::Worker::Worker(ThreadPool *pool) : pool_{pool} {
  std::thread actual_thread([this]() {
    while (true) {
      std::unique_ptr<Job> job = this->pool_->queue_.Pop();

      if (job) {
        job->Execute();
      }

      // must check the size of the queue before existing, as a job
      // might add new job to the queue
      if (this->pool_->queue_.size() == 0 && !this->pool_->open_) {
        this->pool_->threads_.fetch_add(1);
        return;
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

  for (size_t i = 0; i < threads_; ++i) {
    this->Push(std::unique_ptr<Job>{});
  }
}

void ThreadPool::Push(std::unique_ptr<Job> &&job) {
  queue_.Push(std::move(job));
}
}  // namespace threadpool
