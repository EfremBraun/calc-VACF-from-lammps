if (UNIX)
        set(EXEC_INSTALL_PREFIX
                "${CMAKE_INSTALL_PREFIX}"
                CACHE PATH "Base directory for executables and libraries"
                )
endif(UNIX)
