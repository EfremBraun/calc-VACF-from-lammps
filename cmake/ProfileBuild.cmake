# These options are used when we are profiling. 

set(CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS_RELEASE_INIT} -pg" CACHE STRING "Flags for profile builds")
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE_INIT} -pg" CACHE STRING "Flags for profile builds (C++)")
set(CMAKE_LD_FLAGS_PROFILE "${CMAKE_LD_FLAGS_RELEASE_INIT} -pg" CACHE STRING "Flags for profile builds (LD)")
mark_as_advanced( CMAKE_C_FLAGS_PROFILE CMAKE_CXX_FLAGS_PROFILE CMAKE_LD_FLAGS_PROFILE )

