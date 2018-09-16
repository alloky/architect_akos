cmake_minimum_required (VERSION 3.1)

project (Akos)

# default compile flags
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set (CMAKE_CXX_STANDARD 11)
#set(LOG 0)
option(LOGMOD "help string describing LOG" OFF)

if(LOGMOD)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOGMOD")
else()
endif()

if(WINSYS)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN")
else()
endif()

# include libs
include_directories(lib)
include_directories(src)

set(SOURCE_FILES src)
set(TASKS_FILES tasks)

# cmake flags
 set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
 set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
 set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Collect sources
file(GLOB_RECURSE CPP_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${SOURCE_FILES}/*.cpp")
file(GLOB_RECURSE C_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${SOURCE_FILES}/*.c")

# Collect execs
add_executable(task_1 ${CPP_SOURCES} ${C_SOURCES} "tasks/task_1.cpp")
add_executable(task_2 ${CPP_SOURCES} ${C_SOURCES} "tasks/task_2.cpp")

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
include_directories(src)
add_executable(mt_test tests/mt_test.cpp src/mathtools.cpp)

target_link_libraries(mt_test gtest_main)
add_test(NAME mt_test COMMAND mt_test)