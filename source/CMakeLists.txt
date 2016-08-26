#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -lm" )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3" )

set( corrfunc_src   
     main.c
     corrdata.c
     readinput.c
     sample.c
     utils.c
   )

#add_library(math STATIC IMPORTED)

#find_library(M_LIB m)
add_executable(corrfunc ${corrfunc_src})
target_link_libraries(corrfunc m)

install (TARGETS corrfunc DESTINATION bin)

