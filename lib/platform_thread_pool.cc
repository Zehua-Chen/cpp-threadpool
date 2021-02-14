#include "threadpool/platform_thread_pool.h"

using std::unique_ptr;

namespace threadpool {
platform_thread_pool::platform_thread_pool() {

}

void platform_thread_pool::push(unique_ptr<job> &&job) {
}
}
