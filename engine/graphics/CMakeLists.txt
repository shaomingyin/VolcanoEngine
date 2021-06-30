
file(GLOB_RECURSE GRAPHICS_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_library(graphics STATIC ${GRAPHICS_SOURCES})
add_dependencies(graphics game)
target_link_libraries(graphics PUBLIC game)
set_target_properties(graphics PROPERTIES OUTPUT_NAME "VolcanoGraphics")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GRAPHICS_SOURCES})

target_include_directories(graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

configure_file(
    include/Volcano/Graphics/Config.hpp.in
    include/Volcano/Graphics/Config.hpp
    )

find_package(Qt6 COMPONENTS OpenGL Quick REQUIRED)
target_link_libraries(graphics PUBLIC Qt6::OpenGL Qt6::Quick)

include(FindAssimp)
if(ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
    link_libraries(${ASSIMP_LIBRARIES})
else()
    message(FATAL_ERROR "Assimp not found.")
endif()

