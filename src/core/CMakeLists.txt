
set(LIB_NAME CarsEvolutionCore)

set(SRC_CORE
    CarsEvolutionRoot.cc
    World.cc
)

add_library(${LIB_NAME} STATIC ${SRC_CORE})
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
