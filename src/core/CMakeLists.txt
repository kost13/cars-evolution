
set(LIB_NAME CarsEvolutionCore)

set(SRC_CORE
    CarsEvolutionRoot.cc
    World.cc
)

include_directories(../../box2d/include)

add_library(${LIB_NAME} STATIC ${SRC_CORE})
target_link_libraries(${LIB_NAME} PUBLIC box2d)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
