
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

configure_file(
    include/Volcano/Launcher/Config.hpp.in
    include/Volcano/Launcher/Config.hpp
    )

file(GLOB_RECURSE LAUNCHER_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_executable(launcher ${LAUNCHER_SOURCES})
set_target_properties(launcher PROPERTIES OUTPUT_NAME "VolcanoLauncher")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LAUNCHER_SOURCES})
target_link_libraries(launcher PRIVATE runtime)
