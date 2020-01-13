#include <iostream>

#include "threadpool/threadpool.h"

using namespace threadpool;
using std::cout;
using std::endl;
using std::unique_ptr;

std::mutex cout_m;

void TestEmpty() {
  cout << endl << "test empty" << endl;
  ThreadPool pool{3};
}

void TestPushOne() {
  cout << endl << "test push one" << endl;
  ThreadPool pool{3};

  pool.Push(CreateJob([]() {
    std::unique_lock<std::mutex> lock{cout_m};
    cout << "only task" << endl;
  }));
}

void TestPushComplex() {
  cout << endl << "test push complex" << endl;
  ThreadPool pool{3};

  for (size_t i = 0; i < 100; i++) {
    pool.Push(CreateJob([=]() {
      std::unique_lock<std::mutex> lock{cout_m};
      cout << "hello from job " << i  << endl;
    }));
    // pool.Wait();
  }

  pool.Push(CreateJob([&]() {
    std::unique_lock<std::mutex> lock{cout_m};
    cout << "hello from job b!" << endl;

    pool.Push(CreateJob([&]() {
      cout << "hello from job d!" << endl;

      pool.Push(CreateJob([&]() {
        cout << "hello from job e!" << endl;
      }));
    }));
  }));

  // pool.Push(Task([&]() {
  //   std::unique_lock<std::mutex> lock{cout_m};
  //   cout << "hello from job c!" << endl;
  // }));

  // pool.Wait();
}

int main() {
  TestEmpty();
  TestPushOne();
  TestPushComplex();

  std::unique_lock<std::mutex> lock{cout_m};
  cout << "all job done executing" << endl;
}