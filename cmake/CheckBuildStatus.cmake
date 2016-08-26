## this will check and see if we just copied the build-directory
# this is not good, and we should avoid this.

if(CMAKE_HOST_UNIX)

    execute_process(COMMAND hostname
                    OUTPUT_VARIABLE TMP_HOSTNAME
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(BUILD_HOSTNAME AND NOT "${BUILD_HOSTNAME}" STREQUAL "${TMP_HOSTNAME}")
        message(WARNING "
            The CMake cache, probably generated on a different host (${BUILD_HOSTNAME
            is being reused! This could lead to inconsitencies; therefore, it is
            recommended to regenerate the cache!")
    endif()

    set(BUILD_HOSTNAME "${TMP_HOSTNAME}" CACHE INTERNAL "Hostname of the machine where the cache was generated.")

endif()

