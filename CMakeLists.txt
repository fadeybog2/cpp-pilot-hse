project(cpp-pilot-hse)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

include(cmake/BuildFlags.cmake)
include(cmake/TestSolution.cmake)

find_package(Catch REQUIRED)

include_directories(util)

# 0
add_subdirectory(tasks/multiplication)

# 1
add_subdirectory(tasks/associative_operation)
add_subdirectory(tasks/palindrome)
add_subdirectory(tasks/password)
add_subdirectory(tasks/sort_students)

#2
add_subdirectory(tasks/admission)
add_subdirectory(tasks/scorer)
add_subdirectory(tasks/search)
add_subdirectory(tasks/tests_checking)
add_subdirectory(tasks/unixpath)
add_subdirectory(tasks/word2vec)

#3
add_subdirectory(tasks/mines)
add_subdirectory(tasks/scorer2)
add_subdirectory(tasks/search2)
add_subdirectory(tasks/unixpath2)
