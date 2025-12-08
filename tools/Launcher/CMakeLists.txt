
add_executable(Launcher
    include/Volcano/Launcher/Common.h
    include/Volcano/Launcher/Config.h.in
    include/Volcano/Launcher/MainWindow.h
    src/Volcano/Launcher/Main.cpp
    src/Volcano/Launcher/MainWindow.cpp
)

configure_file(
    include/Volcano/Launcher/Config.h.in
    include/Volcano/Launcher/Config.h
)

set_target_properties(Launcher PROPERTIES OUTPUT_NAME VolcanoLauncher)

target_include_directories(Launcher PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(Launcher PRIVATE Game Acoustics Graphics)

volcano_link_qt(Launcher Quick)
volcano_setup_target(Launcher)
