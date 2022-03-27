
set(CMAKE_AUTOUIC ON)

link_libraries(Runtime)

set(VOLCANO_EDITOR_QTCREATOR_CMAKE_DIR "" CACHE PATH "QtCreator cmake directory path.")
find_package(QtCreator COMPONENTS Core ProjectExplorer REQUIRED
    PATHS ${VOLCANO_EDITOR_QTCREATOR_CMAKE_DIR}
    )

glob_sources(EDITOR_SOURCES)

add_qtc_plugin(Editor
    PLUGIN_DEPENDS
        QtCreator::ExtensionSystem
        QtCreator::Core
        QtCreator::ProjectExplorer
        Qt6::Core
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    SOURCES
        ${EDITOR_SOURCES}
    )

#add_dependencies(Editor Server Launcher)
set_target_properties(Editor PROPERTIES OUTPUT_NAME "VolcanoEditor")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCES})

configure_file(
    include/Volcano/Editor/Config.hpp.in
    include/Volcano/Editor/Config.hpp
    )

configure_file(
    include/Volcano/Editor/Plugin.json.in
    include/Volcano/Editor/Plugin.json
    )

# new project wizard

set(PROJECT_WIZARDS_DIR "${CMAKE_BINARY_DIR}/share/qtcreator/templates/wizards/projects/volcanoengine")

configure_file(
    src/Volcano/Editor/Wizards/Project/CMakeLists.txt.in
    ${PROJECT_WIZARDS_DIR}/CMakeLists.txt
    )

configure_file(
    src/Volcano/Editor/Wizards/Project/Wizard.json.in
    ${PROJECT_WIZARDS_DIR}/wizard.json
    )

file(COPY src/Volcano/Editor/Wizards/Project/Game
    DESTINATION ${PROJECT_WIZARDS_DIR}/
    )
