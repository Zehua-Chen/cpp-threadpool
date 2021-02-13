project("threadpool")

add_subdirectory(lib/)

if (DEFINED CPP_THREADPOOL_DEV)
    add_subdirectory(unittests/)
endif()
