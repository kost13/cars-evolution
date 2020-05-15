cmake_minimum_required(VERSION 3.10)

project(CarsEvolution LANGUAGES CXX)

option(BUILD_TESTS "Build the CarsEvolution unit tests" ON)
option(BUILD_GUI "Build the CarsEvolution GUI" ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(extern/CppUtils)
add_subdirectory(extern/box2d/src)
add_subdirectory(app)
add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
