project("threadpool")

add_subdirectory(lib/)

if (DEFINED THREADPOOL_DEV)
  enable_testing()

  add_subdirectory(unittests/)
endif()
