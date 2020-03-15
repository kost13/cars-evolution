cmake_minimum_required(VERSION 3.1)

set(TESTS_NAME CarsEvolutionTests)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(SRC
    main.cc
)

include_directories(../src)

add_executable(${TESTS_NAME} ${SRC})
target_link_libraries(${TESTS_NAME} PUBLIC CarsEvolutionCore ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

