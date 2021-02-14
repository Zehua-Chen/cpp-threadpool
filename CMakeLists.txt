project("threadpool")

add_subdirectory(lib/)

if (DEFINED THREADPOOL_DEV)
  include(cmake/AddTestExecutable.cmake)

  enable_testing()
  add_subdirectory(unittests/)
endif()
