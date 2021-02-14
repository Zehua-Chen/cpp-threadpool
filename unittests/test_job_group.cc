#include <chrono>
#include <future>
#include <iostream>

#include "threadpool/threadpool.h"

using namespace threadpool;
using std::cout;
using std::endl;
using std::future;
using std::shared_ptr, std::make_shared;
using std::unique_ptr;
using namespace std::literals::chrono_literals;

std::mutex cout_m;

void test_empty() {
  cout << endl << "test empty" << endl;
  auto pool = make_shared<default_thread_pool>(3);
}

void test_push_one() {
  cout << endl << "test push one" << endl;
  auto pool = make_shared<default_thread_pool>(3);
  auto group = make_shared<job_group>();

  group->enter();

  pool->push(create_job([=]() {
    std::unique_lock<std::mutex> lock{cout_m};
    cout << "test push one: task 1" << endl;
    group->leave();
  }));

  group->wait();
}

void test_push_complex() {
  cout << endl << "test push complex" << endl;

  auto pool = make_shared<default_thread_pool>(3);
  auto group = make_shared<job_group>();

  for (size_t i = 0; i < 100; i++) {
    group->enter();

    pool->push(create_job([=]() {
      std::unique_lock<std::mutex> lock{cout_m};
      cout << "test push complex: job " << i << endl;

      group->leave();
    }));
  }

  group->enter();

  pool->push(create_job([=]() {
    std::unique_lock<std::mutex> lock{cout_m};
    cout << "test push complex: job b" << endl;

    pool->push(create_job([=]() {
      std::unique_lock<std::mutex> lock{cout_m};
      cout << "test push complex: job d" << endl;

      pool->push(create_job([=]() {
        std::unique_lock<std::mutex> lock{cout_m};
        cout << "test push complex: job e" << endl;
        group->leave();
      }));
    }));
  }));

  group->wait();
}

void test() {
  test_empty();
  test_push_one();
  test_push_complex();

  std::unique_lock<std::mutex> lock{cout_m};
}

int main() {
  future<void> test_future = std::async(test);

  if (test_future.wait_for(2s) == std::future_status::timeout) {
    cout << "timed out!" << endl;
    return -1;
  }

  cout << "all job done executing!" << endl;

  return 0;
}
