cmake_minimum_required(VERSION 3.1)

set(TESTS_NAME CarsEvolutionTests)

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

set(SRC
    main.cc
)

include_directories(../src)

add_executable(${TESTS_NAME} ${SRC})
target_link_libraries(${TESTS_NAME} PUBLIC CarsEvolutionCore ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

