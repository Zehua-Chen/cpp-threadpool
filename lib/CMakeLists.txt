add_library(
    threadpool
    job.cc
    pool.cc)

target_include_directories(
    threadpool
    PUBLIC
      ../include/)

target_compile_features(
    threadpool
    PUBLIC
      cxx_std_17)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND NOT ${WIN32})
    target_compile_options(
        threadpool
        PUBLIC
          "-fsanitize=thread")
endif()