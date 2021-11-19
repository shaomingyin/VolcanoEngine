
link_libraries(Runtime)
link_qt_modules(Widgets OpenGLWidgets)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

file(GLOB_RECURSE EDITOR_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/*.in
    )

qt_add_executable(Editor ${EDITOR_SOURCES})
set_target_properties(Editor PROPERTIES OUTPUT_NAME "VolcanoEditor")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCES})

configure_file(
    include/Volcano/Editor/Config.hpp.in
    include/Volcano/Editor/Config.hpp
    )
