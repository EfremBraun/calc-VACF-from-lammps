# define system dependent compiler flags

if (UNIX)
    # Define GNUCC compiler flags
    if (${CMAKE_C_COMPILER_ID} MATCHES "(GNU|Clang)")


        # add -Wconversion ?
        #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -pedantic -pedantic-errors")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wshadow -Wmissing-prototypes -Wdeclaration-after-statement")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wformat-security")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-format-attribute")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c11 -DDEBUG")
        #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c99 ")

    elseif (${CMAKE_C_COMPILER_ID} MATCHES "icc")

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -w2 -ipo ")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wformat-security")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-format-attribute")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c11")

    endif ()
    #endif (${CMAKE_C_COMPILER_ID} MATCHES "(GNU|Clang)")

endif (UNIX)

