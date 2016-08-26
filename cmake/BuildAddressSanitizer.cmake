# Options for compiling with address santizer.
# this should help weed out those nasty memory problems.
# recuires gcc 4.8

set(_flags "-O1 -g -fsanitize=address -fno-omit-frame-pointer")

foreach(_language C CXX)
        string(REPLACE "X" "+" _human_readable_language ${_language})
        set(CMAKE_${_language}_FLAGS_ASAN ${_flags} CACHE STRING "${_human_readable_language} flags for address sanitizer")
        mark_as_advanced(CMAKE_${_language}_FLAGS_ASAN)
endforeach()

## this is a fix for bug.
string(TOUPPER "${CMAKE_BUILD_TYPE}" _cmake_build_type)
if (APPLE AND _cmake_build_type STREQUAL ASAN) #https://code.google.com/p/address-sanitizer/issues/detail?id=210
           set(BUILD_SHARED_LIBS OFF CACHE BOOL "Disabled for ASAN builds" FORCE)
endif()

