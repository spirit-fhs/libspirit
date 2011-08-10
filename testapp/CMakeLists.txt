cmake_minimum_required (VERSION 2.8)


set (SRCS src/testapp.c)
set (HDRS )
set (PUB_HDRS )

set (EXTRA_LIBS libspirit)

# set up some paths
set (incDir ${CMAKE_CURRENT_BINARY_DIR}/../include)

include_directories (${incDir})



# add the executable
add_executable(testapp ${SRCS} ${HDRS} ${PUB_HDRS})
target_link_libraries (testapp  ${EXTRA_LIBS})

#copy our shared library to executable so it can run
if (CMAKE_MAJOR_VERSION GREATER 2 OR 
   (CMAKE_MAJOR_VERSION EQUAL   2 AND CMAKE_MINOR_VERSION GREATER 8) OR 
   (CMAKE_MAJOR_VERSION EQUAL   2 AND CMAKE_MINOR_VERSION EQUAL   8 AND CMAKE_PATCH_VERSION GREATER 3))
  add_custom_command(TARGET testapp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:libspirit>/$<TARGET_FILE_NAME:libspirit> $<TARGET_FILE_DIR:testapp>/$<TARGET_FILE_NAME:libspirit>)
else()
  message(WARNING "Can't copy libspirit to the directory of the testapp binary file because your CMake Version is too low (less 2.8.4). If testapp won't run you should copy the library by yourself.")
endif()

