
link_qt_modules(Core)
link_libraries(Runtime)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

glob_sources(LAUNCHER_SOURCES)
qt_add_executable(Launcher ${LAUNCHER_SOURCES})
set_target_properties(Launcher PROPERTIES OUTPUT_NAME "VolcanoLauncher")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LAUNCHER_SOURCES})

configure_file(
    include/Volcano/Launcher/Config.hpp.in
    include/Volcano/Launcher/Config.hpp
    )
