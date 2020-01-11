add_executable(
    test_basic
    test_basic.cc)

target_link_libraries(
    test_basic
    PUBLIC
      threadpool)