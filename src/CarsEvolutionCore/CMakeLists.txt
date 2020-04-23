
set(LIB_NAME CarsEvolutionCore)

set(SRC_CORE
    CarsEvolutionRoot.cc
    CarsEvolutionRoot.h
    CarsPopulationData.cc
    CarsPopulationData.h
    SimulationData.cc
    SimulationData.h

    Physics/World.cc
    Physics/World.h

    Evolution/Evolution.cc
    Evolution/Evolution.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${LIB_NAME} STATIC ${SRC_CORE})
target_link_libraries(${LIB_NAME} PUBLIC box2d CppUtils)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${LIB_NAME} PROPERTIES CXX_STANDARD 14)

if (CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra)
elseif(MSVC)
    target_compile_options(${LIB_NAME} PRIVATE /W3)
endif()
