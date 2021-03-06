
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_GUI)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    find_package(Qt5 COMPONENTS Core Widgets Charts Quick QuickControls2 REQUIRED)    
endif()

set(SRC
    main.cc
    AppBuilder.cc
    AppBuilder.h
    AppInterface.cc
    AppInterface.h
    FileIO.cc
    FileIO.h
    CarsPopulationModel.cc
    CarsPopulationModel.h
    JsonParser.cc
    JsonParser.h
    EvolutionSettingsModel.cc
    EvolutionSettingsModel.h
)

set(RESOURCES
    qml.qrc
)

add_executable(${PROJECT_NAME} ${SRC} ${RESOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE CarsEvolutionCore CppUtils)

if(BUILD_GUI)
        target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets Qt5::Quick Qt5::QuickControls2 Qt5::Charts)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
endif()
