
set(LIB_NAME CarsEvolutionCore)

set(SRC_CORE
    CarsEvolutionRoot.cc
    CarsEvolutionRoot.h
    CarsPopulationData.cc
    CarsPopulationData.h
    SimulationData.cc
    SimulationData.h
    ParametersMatrix.cc
    ParametersMatrix.h

    Physics/World.cc
    Physics/World.h
    Physics/Car.h
    Physics/Car.cpp

    Evolution/Evolution.cc
    Evolution/Evolution.h
    Evolution/MathUtils.cc
    Evolution/MathUtils.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${LIB_NAME} STATIC ${SRC_CORE})
target_link_libraries(${LIB_NAME} PUBLIC box2d CppUtils gcov)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${LIB_NAME} PROPERTIES CXX_STANDARD 14)

if (CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        target_link_libraries(${LIB_NAME} PUBLIC gcov)
        target_compile_options(${LIB_NAME} PRIVATE -g -O0 -Wall -fprofile-arcs -ftest-coverage)
    endif()
elseif(MSVC)
    target_compile_options(${LIB_NAME} PRIVATE /W3)
endif()
