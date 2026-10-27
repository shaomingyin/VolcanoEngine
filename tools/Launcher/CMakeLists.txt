
add_executable(Launcher
    include/Volcano/Launcher/Common.h
    include/Volcano/Launcher/Config.h.in
    include/Volcano/Launcher/Console.h
    src/Volcano/Launcher/Console.cpp
    src/Volcano/Launcher/Main.cpp
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

volcano_target_setup(Launcher)
target_link_libraries(Launcher PRIVATE Gameplay)
target_link_libraries(Launcher PRIVATE Gui FrameworkLocal)

find_package(argh REQUIRED)
target_link_libraries(Launcher PUBLIC argh)
