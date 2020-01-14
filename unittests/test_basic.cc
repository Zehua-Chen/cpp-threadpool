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
  JobGroup group;

  group.Enter();

  pool.Push(CreateJob([&]() {
    std::unique_lock<std::mutex> lock{cout_m};
    cout << "test push one: task 1" << endl;
    group.Leave();
  }));

  group.Wait();
}

void TestPushComplex() {
  cout << endl << "test push complex" << endl;
  ThreadPool pool{3};
  std::shared_ptr<JobGroup> group = std::make_shared<JobGroup>();

  for (size_t i = 0; i < 100; i++) {
    group->Enter();

    pool.Push(CreateJob([=]() {
      std::unique_lock<std::mutex> lock{cout_m};
      cout << "test push complex: job " << i  << endl;

      group->Leave();
    }));
  }

  group->Enter();

  pool.Push(CreateJob([&]() {
    std::unique_lock<std::mutex> lock{cout_m};
    cout << "test push complex: job b" << endl;

    pool.Push(CreateJob([&]() {
      cout << "test push complex: job d" << endl;

      pool.Push(CreateJob([&]() {
        cout << "test push complex: job e" << endl;
        group->Leave();
      }));
    }));
  }));

  group->Wait();
}

int main() {
  TestEmpty();
  TestPushOne();
  TestPushComplex();

  std::unique_lock<std::mutex> lock{cout_m};
  cout << "all job done executing" << endl;
}