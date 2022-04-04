
set(CMAKE_AUTOUIC ON)

link_libraries(Runtime)
link_qt_modules(Widgets)

set(VOLCANO_EDITOR_QTC_CMAKE_DIR "" CACHE PATH "QtCreator cmake directory path.")
find_package(QtCreator COMPONENTS Core ProjectExplorer REQUIRED PATHS ${VOLCANO_EDITOR_QTC_CMAKE_DIR})

set(VOLCANO_EDITOR_PROJECT_MIMETYPE "application/x-volcano-project")
set(VOLCANO_EDITOR_WORLD_MIMETYPE "application/x-volcano-world")
set(VOLCANO_EDITOR_SCRIPT_MIMETYPE "application/x-volcano-script")

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

add_dependencies(Editor Compiler Linker Server Launcher)
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

# custom json wizard templates

set(QTC_WIZARDS_DIR "${CMAKE_BINARY_DIR}/share/qtcreator/templates/wizards")

configure_file(
    src/Volcano/Editor/Wizards/Projects/Game/Wizard.json.in
    ${QTC_WIZARDS_DIR}/projects/volcanogame/wizard.json
    )

configure_file(
    src/Volcano/Editor/Wizards/Projects/Game/Manifest.vpf.in
    ${QTC_WIZARDS_DIR}/projects/volcanogame/Manifest.vpf
    )

configure_file(
    src/Volcano/Editor/Wizards/Projects/Module/Wizard.json.in
    ${QTC_WIZARDS_DIR}/projects/volcanomodule/wizard.json
    )

configure_file(
    src/Volcano/Editor/Wizards/Projects/Module/Manifest.vpf.in
    ${QTC_WIZARDS_DIR}/projects/volcanomodule/Manifest.vpf
    )

configure_file(
    src/Volcano/Editor/Wizards/World/Wizard.json.in
    ${QTC_WIZARDS_DIR}/volcanoworld/wizard.json
    )
