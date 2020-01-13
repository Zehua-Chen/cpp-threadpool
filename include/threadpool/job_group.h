#pragma once

namespace threadpool {
class JobGroup {
 public:
  void Enter();
  void Leave();

  void Wait();

 private:
};
}