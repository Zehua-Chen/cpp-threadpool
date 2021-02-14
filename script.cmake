# include(CMakeParseArguments)

function(add_threadpool_test test)
    set(OPTIONS MALE FEMALE)
    set(PREFIX TEST)

    cmake_parse_arguments(PARSE_ARGV 1 "${PREFIX}" "${OPTIONS}" "FILE;GENDER" "FILES")

    if (DEFINED TEST_MALE)
        message("${TEST_MALE}")
    else()
        message("MALE not defined")
    endif()

    if (DEFINED TEST_FILES)
        message("${TEST_FILES}")
    else()
        message("FILES not defined")
    endif()
endfunction()

add_threadpool_test(test_a FILE a.cc GENDER Female FILES b.cc c.cc)

# set(prefix PREFIX_)
# set(${prefix}VAR "foo")
# message(DEBUG " value of " "${prefix}VAR" " is " ${${prefix}VAR})

# function(func)
#     message("func()")
#     set(prefix ARG)
#     set(options OPTION)
#     set(oneValueArgs VALUE)
#     set(multiValueArgs MULTIVALUE)
#     cmake_parse_arguments(PARSE_ARGV 0 "${prefix}" "${options}" "${oneValueArgs}" "${multiValueArgs}")

#     # Add additional underscore to the variables expanded here!
#     message(DEBUG ${${prefix}_UNPARSED_ARGUMENTS})
#     message(DEBUG ${${prefix}_OPTION})
#     message(DEBUG ${${prefix}_VALUE})
#     message(DEBUG ${${prefix}_MULTIVALUE})
# endfunction(func)

# func(VALUE 42 MULTIVALUE "foo" "bar" "baz")

# function(foo)
#   message("in foo: ${ARGN}")
#   cmake_parse_arguments("" "" "ONE" "" ${ARGN})

#   if (DEFINED _UNPARSED_ARGUMENTS)
#     message("UNPARSED_ARGUMENTS: ${_UNPARSED_ARGUMENTS}")
#   endif()

#   if (DEFINED _ONE)
#     message("ONE: ${_ONE}")
#   endif()
# endfunction()

# foo(ONE 1)
# foo(ONE 1 1)
# foo(ONE)
