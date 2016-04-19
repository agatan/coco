cmake_minimum_required(VERSION 3.2)

project(coco CXX)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG")

include_directories(include)

find_package(Boost 1.59.0 REQUIRED COMPONENTS system)
if (!${Boost_FOUND})
  message(FATAL_ERROR "Boost library not found.")
endif()
include_directories(${Boost_INCLUDE_DIRS})

if(NOT without-test)
  enable_testing()
  add_subdirectory(test)
endif()

