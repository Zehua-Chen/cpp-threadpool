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