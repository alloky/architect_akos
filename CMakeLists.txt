cmake_minimum_required (VERSION 2.6)

project (Akos)

# default compile flags
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall")


if(LOG)
	include_directories(lib)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOG")
endif()

# cmake flags
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(main src/main.cpp)