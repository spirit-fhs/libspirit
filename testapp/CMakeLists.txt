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
add_custom_command(TARGET testapp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:libspirit>/$<TARGET_FILE_NAME:libspirit> $<TARGET_FILE_DIR:testapp>/$<TARGET_FILE_NAME:libspirit>)