
add_executable(launcher)

add_dependencies(launcher base)
set_target_properties(launcher PROPERTIES OUTPUT_NAME "VolcanoLauncher")

target_include_directories(launcher
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_link_libraries(launcher
    PRIVATE
        base
    )

target_sources(launcher
    PRIVATE
        include/Volcano/Launcher/Common.hpp
        include/Volcano/Launcher/MainWindow.hpp
        src/Volcano/Launcher/Main.cpp
        src/Volcano/Launcher/MainWindow.cpp
    )

get_target_property(LAUNCHER_SOURCES launcher SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LAUNCHER_SOURCES})
