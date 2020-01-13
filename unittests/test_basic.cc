#include <iostream>

#include "threadpool/threadpool.h"

using namespace threadpool;
using std::cout;
using std::endl;
using std::unique_ptr;

std::mutex cout_m;

int main() {
  ThreadPool pool{3};

  for (size_t i = 0; i < 100; i++) {
    pool.Push(Task([=]() {
      std::unique_lock<std::mutex> lock{cout_m};
      cout << "hello from job " << i  << endl;
    }));
    // pool.Wait();
  }

  pool.Push(Task([&]() {
    std::unique_lock<std::mutex> lock{cout_m};
    cout << "hello from job b!" << endl;

    pool.Push(Task([&]() {
      cout << "hello from job d!" << endl;

      pool.Push(Task([&]() {
        cout << "hello from job e!" << endl;
      }));
    }));
  }));

  // pool.Push(Task([&]() {
  //   std::unique_lock<std::mutex> lock{cout_m};
  //   cout << "hello from job c!" << endl;
  // }));

  // pool.Wait();

  std::unique_lock<std::mutex> lock{cout_m};
  cout << "all job done executing" << endl;
}