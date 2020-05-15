cmake_minimum_required(VERSION 3.10)

project(CarsEvolution LANGUAGES CXX)

option(BUILD_TESTS "Build the CarsEvolution unit tests" ON)
option(BUILD_GUI "Build the CarsEvolution GUI" ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(CppUtils)
add_subdirectory(box2d/src)
add_subdirectory(app)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
