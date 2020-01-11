#pragma once

namespace threadpool {
class Job {
 public:
  virtual void Execute() = 0;
};
}  // namespace threadpool