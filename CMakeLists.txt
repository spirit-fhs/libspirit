cmake_minimum_required (VERSION 2.8)
project (libspirit)


# The version number.
set (libspirit_VERSION_MAJOR 1)
set (libspirit_VERSION_MINOR 0)

# the name of the yajl directory
set (YAJL_DIST_NAME yajl-2.0.3)


# set the library type to dynamic (otherwise STATIC)
set (LIB_TYPE SHARED)



# needed files are organized in lists
set (incDir ${PROJECT_BINARY_DIR}/include/libspirit)

set (SRCS src/libspirit.c
          src/libspirit_error.c
          src/libspirit_news.c
          src/libspirit_option.c
          src/libspirit_schedule.c
          src/utility.c)
set (HDRS src/libspirit_data.h
          src/libspirit_option.h
          src/libspirit.h
          src/utility.h)
set (PUB_HDRS src/api/spirit.h
              src/api/spirit_error.h
              src/api/spirit_news.h
              src/api/spirit_schedule.h)

# the name of the yajl directory
set (YAJL_DIST_NAME yajl-2.0.3)


# set the library type to dynamic (otherwise STATIC)
set (LIB_TYPE SHARED)


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/libspiritconfig.h.in"
  "${PROJECT_BINARY_DIR}/libspiritconfig.h")


# add the YAJL json library
add_subdirectory (yajl)
include_directories ("${PROJECT_BINARY_DIR}/yajl/${YAJL_DIST_NAME}/include")
set (EXTRA_LIBS ${EXTRA_LIBS} yajl_s)

# add libcurl
set (CURL_STATICLIB ON CACHE BOOL "(FORCED) get a static libcurl" FORCE)
set (BUILD_CURL_EXE OFF CACHE BOOL "(FORCED) no exe needed" FORCE)
set (BUILD_CURL_TESTS OFF CACHE BOOL "(FORCED) no tests needed" FORCE)
if(MSVC)
  set (BUILD_RELEASE_DEBUG_DIRS ON CACHE BOOL "(FORCED) seems to be bugged when off" FORCE)
endif()

add_subdirectory (curl)
include_directories ("${PROJECT_BINARY_DIR}/curl/include/curl")
include_directories ("${PROJECT_BINARY_DIR}/curl/lib")
include_directories ("${PROJECT_SOURCE_DIR}/curl/include")
include_directories ("${PROJECT_BINARY_DIR}/yajl/${YAJL_DIST_NAME}/include")
set (EXTRA_LIBS ${EXTRA_LIBS} libcurl)


# add the library
add_library(libspirit ${LIB_TYPE} ${SRCS} ${HDRS} ${PUB_HDRS})
target_link_libraries (libspirit  ${EXTRA_LIBS})

#copy public headers
file (MAKE_DIRECTORY ${incDir})
foreach (header ${PUB_HDRS})
  set (header ${PROJECT_SOURCE_DIR}/${header})
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${header} ${incDir})
endforeach (header ${PUB_HDRS})

# include api headers and generated config file
include_directories (${incDir}/..)
include_directories(${PROJECT_BINARY_DIR})


#unset the name prefix, we want it to be called libspirit even in windows
set_target_properties(libspirit PROPERTIES PREFIX "")

#build testapp
add_subdirectory(testapp)


#set compiler flag for static curl
if(MSVC)
  set_property(TARGET libspirit PROPERTY COMPILE_DEFINITIONS CURL_STATICLIB)
else()
  add_definitions(-DCURL_STATICLIB)
endif()


# add the install targets
install (TARGETS libspirit DESTINATION lib)
install (FILES ${PUB_HDRS} DESTINATION include/libspirit)


# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set (CPACK_PACKAGE_VERSION_MAJOR "${libspirit_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${libspirit_VERSION_MINOR}")
include (CPack)