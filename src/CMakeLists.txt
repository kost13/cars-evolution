cmake_minimum_required(VERSION 3.1)


#set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(.)

if(BUILD_GUI)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    find_package(Qt5 COMPONENTS Core Quick REQUIRED)
#    add_definitions(-DGUI_APP)
endif()

find_package(Boost 1.72.0 COMPONENTS thread test signals2)
include_directories(${Boost_INCLUDE_DIRS})


set(SRC
    main.cc
    Worker.cc
    AppInterface.cc
)

set(RESOURCES
    qml.qrc
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../box2d/include)

add_subdirectory(core)

add_executable(${PROJECT_NAME} ${SRC} ${RESOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC CarsEvolutionCore box2d ${Boost_INCLUDE_DIRS})

if(BUILD_GUI)
	target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick)
endif()
