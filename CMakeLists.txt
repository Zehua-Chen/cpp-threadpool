project("threadpool")

add_subdirectory(lib/)

if (DEFINED CPP_THREADPOOL_DEV)
  enable_testing()

  add_subdirectory(unittests/)
endif()
