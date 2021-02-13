add_executable(
    test_job_group
    test_job_group.cc)

target_link_libraries(
    test_job_group
    PUBLIC
      threadpool)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND NOT ${WIN32})
    target_compile_options(
        test_job_group
        PUBLIC
        "-fsanitize=thread")

    target_link_options(
        test_job_group
        PUBLIC
        "-fsanitize=thread")
endif()
