# - add_cunit_test(test_name test_source linklib1 ... linklibN)


enable_testing()

find_package(CUnit REQUIRED)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS_PROFILING "-g -O0 -Wall -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wwrite-strings -fprofile-arcs -ftest-coverage" CACHE STRING "Profiling Compiler Flags")
endif(CMAKE_COMPILER_IS_GNUCC)

function (ADD_CUNIT_TEST _testName _testSource)
    add_executable(${_testName} ${_testSource})
    target_link_libraries(${_testName} ${ARGN} ${CUNIT_LIBRARY})
    add_test(${_testName} ${CMAKE_CURRENT_BINARY_DIR}/${_testName})
endfunction (ADD_CUNIT_TEST)
