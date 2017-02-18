project (lightsoffsolver C)
cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
find_package (PkgConfig)

set (TARGET lightsoffsolver)
set (SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set (INCLUDE_DIRS ${INCLUDE_DIRS} ${SRC_DIR})
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -pedantic -O2")

file (GLOB_RECURSE SRC ${SRC_DIR}/*.c)

pkg_check_modules (GDK-PIXBUF2 REQUIRED gdk-pixbuf-2.0)

include_directories (${INCLUDE_DIRS}
                     ${GDK-PIXBUF2_INCLUDE_DIRS})

add_executable (${TARGET} ${SRC})

target_link_libraries (${TARGET}
                       ${GDK-PIXBUF2_LIBRARIES})
