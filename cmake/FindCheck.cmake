# - Try to find the CHECK libraries
# Once done this will define
#
#  CHECK_FOUND - system has check
#  CHECK_INCLUDE_DIR - the check include directory
#  CHECK_LIBRARIES - check library

IF ( CHECK_INCLUDE_DIR AND CHECK_LIBRARIES )
    SET( Check_FIND_QUIETLY TRUE )
ENDIF ( CHECK_INCLUDE_DIR AND CHECK_LIBRARIES )

IF ( NOT WIN32 )
   INCLUDE( UsePkgConfig )
   PKGCONFIG( check _LibCHECKIncDir _LibCHECKLinkDir _LibCHECKLinkFlags _LibCHECKCflags )
ENDIF ( NOT WIN32 )

# Look for CHECK include dir and libraries
FIND_PATH( CHECK_INCLUDE_DIR check.h PATHS ${_LibCHECKIncDir} )

FIND_LIBRARY( CHECK_LIBRARIES NAMES check PATHS ${_LibCHECKLinkDir} )

IF ( CHECK_INCLUDE_DIR AND CHECK_LIBRARIES )
        SET( CHECK_FOUND 1 )
        IF ( NOT Check_FIND_QUIETLY )
                MESSAGE ( STATUS "Found CHECK: ${CHECK_LIBRARIES}" )
        ENDIF ( NOT Check_FIND_QUIETLY )
ELSE ( CHECK_INCLUDE_DIR AND CHECK_LIBRARIES )
        IF ( Check_FIND_REQUIRED )
                MESSAGE( SEND_ERROR "Could NOT find CHECK" )
        ELSE ( Check_FIND_REQUIRED )
                IF ( NOT Check_FIND_QUIETLY )
                        MESSAGE( STATUS "Could NOT find CHECK" )       
                ENDIF ( NOT Check_FIND_QUIETLY )
        ENDIF ( Check_FIND_REQUIRED )
ENDIF ( CHECK_INCLUDE_DIR AND CHECK_LIBRARIES )


# Hide advanced variables from CMake GUIs
MARK_AS_ADVANCED( CHECK_INCLUDE_DIR CHECK_LIBRARIES )

