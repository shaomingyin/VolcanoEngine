
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

find_package(Qt6 COMPONENTS Gui Widgets Quick OpenGLWidgets REQUIRED)
link_libraries(Qt6::Gui Qt6::Widgets Qt6::Quick Qt6::OpenGLWidgets)

link_libraries(game graphics sound)

file(GLOB_RECURSE EDITOR_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_executable(editor ${EDITOR_SOURCES})
add_dependencies(editor launcher server)
set_target_properties(editor PROPERTIES OUTPUT_NAME "VolcanoEditor")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCES})

configure_file(
    include/Volcano/Editor/Config.hpp.in
    include/Volcano/Editor/Config.hpp
    )

include(FindAssimp)
if(ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
    link_libraries(${ASSIMP_LIBRARIES})
else()
    message(FATAL_ERROR "Assimp not found.")
endif()
