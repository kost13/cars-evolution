cmake_minimum_required(VERSION 3.1)

if(WIN32)
        if(NOT BOOST_DIR)
            message(FATAL_ERROR " Set BOOST_DIR wit path to boost root dir e.g. C:/boost")
        else()
            set(Boost_INCLUDE_DIRS ${BOOST_DIR})
        endif()
        if(CMAKE_BUILD_TYPE MATCHES Debug)
            if(NOT BOOST_TEST_LIB_DEBUG)
                message(FATAL_ERROR " Set BOOST_TEST_LIB_DEBUG wit path to boost unit test debug library e.g. C:/boost/lib/libboost_unit_test_framework-vc140-mt-gd-x64-1_72.lib")
            else()
                set(Boost_UNIT_TEST_FRAMEWORK_LIBRARY ${BOOST_TEST_LIB_DEBUG})
            endif()
        else()
            if(NOT BOOST_TEST_LIB_RELEASE)
                message(FATAL_ERROR " Set BOOST_TEST_LIB_RELEASE wit path to boost unit test library e.g. C:/boost/lib/libboost_unit_test_framework-vc140-mt-x64-1_72.lib")
            else()
                set(Boost_UNIT_TEST_FRAMEWORK_LIBRARY ${BOOST_TEST_LIB_RELEASE})
            endif()
        endif()
else()
	find_package(Boost COMPONENTS unit_test_framework REQUIRED)
endif()

include_directories(${Boost_INCLUDE_DIRS})

include_directories(../src)

set(CORE_TESTS_NAME core_tests)
add_executable(${CORE_TESTS_NAME} CarsEvolutionCoreTests.cc)
target_link_libraries(${CORE_TESTS_NAME} PUBLIC CarsEvolutionCore ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

set(PHYSICS_TESTS_NAME physics_tests)
add_executable(${PHYSICS_TESTS_NAME} PhysicsTests.cc)
target_link_libraries(${PHYSICS_TESTS_NAME} PUBLIC CarsEvolutionCore ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

set(EVOLUTION_TESTS_NAME evolution_tests)
add_executable(${EVOLUTION_TESTS_NAME} EvolutionTests.cc)
target_link_libraries(${EVOLUTION_TESTS_NAME} PUBLIC CarsEvolutionCore ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})



