project(corrfunc C)

cmake_minimum_required(VERSION 2.8)

# set global var.
set(APPLICATION_NAME  ${PROJECT_NAME})
set(VERBATIM ON)

# for defined macros etc
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

## define some directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

SET(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)

set(PROJECT_VERSION "0.1b")

# always include srcdir and builddir in include path
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(ProfileBuild)   ## add profiler flags
include(BuildAddressSanitizer)  # compile with address sanitizer, remove pesky memory bugs

## define the build-type we are looking at
if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build: Debug Release Profile MemAnalysis." FORCE)
endif()

enable_language(C)

# get version from git
set(SOURCE_IS_GIT_REPOSITORY OFF)
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
        set(SOURCE_IS_GIT_REPOSITORY ON)
endif()

## check to see if we have an old build from a different computer
include(CheckBuildStatus)

# define the root path 
get_filename_component(CORRFUNC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

## put the include dirs which are in the source or the build tree before all other include dirs
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

## check for different types of sse
include(CheckCompilerFlags)

## add some definitions
include(DefineCMakeDefaults)
include(DefinePlatformDefaults)
include(DefineCompilerFlags)
include(DefineInstallationPaths)
include(CheckCCompilerFlag)

if( NOT CMAKE_BUILD_TYPE )
        set( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release MinSizeRel" FORCE)
endif()

# add to flags if debug
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D DEBUG ")

# avoid in-source build (should never happen), shame on you if you do
include(MacroEnsureOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} should be built out-of-source")

## add code
add_subdirectory(source)

message(STATUS "**********************************************")
message(STATUS "*   ${PROJECT_NAME} build opt:")
message(STATUS "*  Unit testing:   ${WITH_TESTING}")
message(STATUS "*  Build type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "*  Get GIT-version ${SOURCE_IS_GIT_REPOSITORY}")
message(STATUS "*  Binary dir:     ${CMAKE_BINARY_DIR}")
message(STATUS "*  Instal dir:     ${CMAKE_INSTALL_PREFIX}")
message(STATUS "*  COMPILER        ${CMAKE_C_COMPILER}")
message(STATUS "*  COMPILER ID     ${CMAKE_C_COMPILER_ID}")
message(STATUS "*  AVX             ${HAS_AVX_EXTENSIONS}")
message(STATUS "*  SSE3            ${HAS_SSE3_EXTENSIONS}")
message(STATUS "*  SSE2            ${HAS_SSE2_EXTENSIONS}")
message(STATUS "*  SSE             ${HAS_SSE_EXTENSIONS}")
message(STATUS "*  CFLAGS          ${CMAKE_C_FLAGS}")
message(STATUS "**********************************************")
                                                                



