cmake_minimum_required(VERSION 3.10)

project(CarsEvolution LANGUAGES CXX)

option(BUILD_TESTS "Build the CarsEvolution unit tests" ON)
option(BUILD_GUI "Build the CarsEvolution GUI" ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -pthread")

add_subdirectory(CppUtils)
add_subdirectory(box2d/src)
add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
