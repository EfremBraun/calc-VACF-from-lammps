# includes srcdir and builddir in include path.
# saves a lot of typing
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# include dirs in source or build tree over other include dirs. 
# will prefere dirs in cureent project
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

# use colored output
set(CMAKE_COLOR_MAKEFILE ON)
