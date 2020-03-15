cmake_minimum_required(VERSION 3.1)

include_directories(.)

if(BUILD_GUI)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    find_package(Qt5 COMPONENTS Core Quick REQUIRED)
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


set(SRC
    main.cc
    Worker.cc
    AppInterface.cc
)

set(RESOURCES
    qml.qrc
)



add_subdirectory(core)

add_executable(${PROJECT_NAME} ${SRC} ${RESOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC CarsEvolutionCore)
#target_link_libraries(${PROJECT_NAME} PUBLIC CarsEvolutionCore box2d ${Boost_INCLUDE_DIRS})

if(BUILD_GUI)
	target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick)
endif()
