# Thread Pool for C++

Thread pool implementation for C++

## Quick Start

```cpp
#include <memory>
#include <iostream>
#include "threadpool/threadpool.h"

using std::cout;
using std::endl;
using std::shared_ptr, std::make_shared;
using namespace threadpool;

int main() {
  shared_ptr<thread_pool> pool = make_shared<thread_pool>(3);
  shared_ptr<job_group> group = make_shared<job_group>();

  group->enter();

  pool->push(create_job([=]() {
    std::unique_lock<std::mutex> lock{cout_m};
    cout << "test push one: task 1" << endl;
    group->leave();
  }));

  group->wait();

  return 0;
}
```

## Development

```
cmake -DCPP_THREADPOOL_DEV=1 ..
```

Defining CPP_THREADPOOL_DEV would enable testing
