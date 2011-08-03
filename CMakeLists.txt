cmake_minimum_required (VERSION 2.8)
project (libspirit)


# The version number.
set (libspirit_VERSION_MAJOR 1)
set (libspirit_VERSION_MINOR 0)

# the name of the yajl directory
set (YAJL_DIST_NAME yajl-2.0.3)


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


# add the yajl json library
#if (USE_YAJL)
  
  add_subdirectory (yajl)
  include_directories ("${PROJECT_BINARY_DIR}/yajl/${YAJL_DIST_NAME}/include")
  set (EXTRA_LIBS ${EXTRA_LIBS} yajl_s)
#endif (USE_YAJL)

set (CURL_STATICLIB ON CACHE BOOL "get a static libcurl" FORCE)
set (BUILD_CURL_EXE OFF CACHE BOOL "no exe needed" FORCE)
set (BUILD_CURL_TESTS OFF CACHE BOOL "no tests needed" FORCE)
set (BUILD_RELEASE_DEBUG_DIRS ON CACHE BOOL "seems to be bugged when off" FORCE)
add_subdirectory (curl)
set (EXTRA_LIBS ${EXTRA_LIBS} libcurl)


# add the executable
add_library(libspirit ${LIB_TYPE} "src/libspirit.c")
target_link_libraries (libspirit  ${EXTRA_LIBS})

#unset the name prefix, we want it to be called libspirit even in windows
set_target_properties(libspirit PROPERTIES PREFIX "")


# add the install targets
install (TARGETS libspirit DESTINATION lib)
install (FILES "${PROJECT_BINARY_DIR}/libspiritconfig.h" 
  DESTINATION include)


# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE  
     "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${libspirit_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${libspirit_VERSION_MINOR}")
include (CPack)