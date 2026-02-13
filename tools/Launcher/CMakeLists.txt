
add_executable(Launcher
    include/Volcano/Launcher/Client.h
    include/Volcano/Launcher/Common.h
    include/Volcano/Launcher/Config.h.in
    include/Volcano/Launcher/Console.h
    include/Volcano/Launcher/Local.h
    include/Volcano/Launcher/Window.h
    src/Volcano/Launcher/Client.cpp
    src/Volcano/Launcher/Console.cpp
    src/Volcano/Launcher/Local.cpp
    src/Volcano/Launcher/Main.cpp
    src/Volcano/Launcher/Window.cpp
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
target_link_libraries(Launcher PUBLIC World Graphics Gui)

find_package(argh REQUIRED)
target_link_libraries(Launcher PUBLIC argh)

find_package(SFML REQUIRED)
target_link_libraries(Launcher PUBLIC sfml::sfml)

find_package(PhysFS REQUIRED)
target_link_libraries(Launcher PUBLIC physfs-static)
