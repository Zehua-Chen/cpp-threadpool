add_executable(
    test_basic
    test_basic.cc)

target_link_libraries(
    test_basic
    PUBLIC
      threadpool)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND NOT ${WIN32})
    target_compile_options(
        test_basic
        PUBLIC
        "-fsanitize=thread")

    target_link_options(
        test_basic
        PUBLIC
        "-fsanitize=thread")
endif()
