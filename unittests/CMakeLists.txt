add_executable(
  test_job_group
  test_job_group.cc)

add_executable(
  test_platform_thread_pool
  test_platform_thread_pool.cc)

target_link_libraries(
  test_job_group
  PUBLIC
    threadpool)

target_link_libraries(
  test_platform_thread_pool
  PUBLIC
    threadpool)

add_test(NAME test_job_group COMMAND test_job_group)
add_test(NAME test_platform_thread_pool COMMAND test_platform_thread_pool)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND (NOT DEFINED WIN32))
  target_compile_options(
    test_job_group
    PUBLIC
    "-fsanitize=thread")

  target_link_options(
      test_job_group
      PUBLIC
      "-fsanitize=thread")

  target_compile_options(
    test_platform_thread_pool
    PUBLIC
    "-fsanitize=thread")

  target_link_options(
    test_platform_thread_pool
      PUBLIC
      "-fsanitize=thread")
endif()
