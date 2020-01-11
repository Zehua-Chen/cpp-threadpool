#include <iostream>

#include "threadpool/job.h"

using namespace threadpool;
using std::cout;
using std::endl;
using std::unique_ptr;

int main() {
  unique_ptr<Job> job = Task([]() { cout << "hello threadpool!" << endl; });

  job->Execute();
}