
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

link_libraries(game graphics sound)

find_package(Qt6 COMPONENTS Gui Quick REQUIRED)
link_libraries(Qt6::Gui Qt6::Quick)

file(GLOB_RECURSE LAUNCHER_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_executable(launcher ${LAUNCHER_SOURCES})
add_dependencies(launcher game graphics sound)
set_target_properties(launcher PROPERTIES OUTPUT_NAME "VolcanoLauncher")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LAUNCHER_SOURCES})

configure_file(
    include/Volcano/Launcher/Config.hpp.in
    include/Volcano/Launcher/Config.hpp
    )

