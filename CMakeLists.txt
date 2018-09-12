cmake_minimum_required (VERSION 2.6)

project (Akos)

# default compile flags
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall")
#set(LOG 0)

if(LOG)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOG")
endif()

# include libs
include_directories(lib)
include_directories(src)

set(SOURCE_FILES src)

# cmake flags
 set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
 set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
 set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${SOURCE_FILES}/*.cpp")
add_executable(main ${SOURCES})
