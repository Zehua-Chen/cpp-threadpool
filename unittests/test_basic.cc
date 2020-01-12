#include <iostream>

#include "threadpool/threadpool.h"

using namespace threadpool;
using std::cout;
using std::endl;
using std::unique_ptr;

int main() {
  ThreadPool pool{2};
  std::mutex cout_m;

  pool.Push(Task([&]() {
    std::unique_lock<std::mutex> lock{cout_m};
    cout << "hello job a!" << endl;
  }));

  pool.Push(Task([&]() {
    std::unique_lock<std::mutex> lock{cout_m};
    cout << "hello job b!" << endl;

    pool.Push(Task([&]() {
      cout << "hello from job c!" << endl;
    }));
  }));

  std::unique_lock<std::mutex> lock{cout_m};
  cout << "all job done executing" << endl;
}