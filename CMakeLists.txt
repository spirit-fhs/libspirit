cmake_minimum_required (VERSION 2.6)
project (libspirit)

# The version number.
set (libspirit_VERSION_MAJOR 1)
set (libspirit_VERSION_MINOR 0)

# set the library type to dynamic (otherwise STATIC)
set (LIB_TYPE SHARED)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/libspiritconfig.h.in"
  "${PROJECT_BINARY_DIR}/libspiritconfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find libspiritconfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_library(libspirit ${LIB_TYPE} "src/libspirit.c")

#unset the name prefix, we want it to be called libspirit even in windows
set_target_properties(libspirit PROPERTIES PREFIX "")

# add the install targets
install (TARGETS libspirit DESTINATION lib)
install (FILES "${PROJECT_BINARY_DIR}/libspiritconfig.h" 
  DESTINATION include)