
add_executable(Launcher
    include/Volcano/Launcher/Client.hpp
    include/Volcano/Launcher/Common.hpp
    include/Volcano/Launcher/Config.hpp.in
    include/Volcano/Launcher/Console.hpp
    include/Volcano/Launcher/Local.hpp
    include/Volcano/Launcher/Window.hpp
    src/Volcano/Launcher/Client.cpp
    src/Volcano/Launcher/Console.cpp
    src/Volcano/Launcher/Local.cpp
    src/Volcano/Launcher/Main.cpp
    src/Volcano/Launcher/Window.cpp
)

configure_file(
    include/Volcano/Launcher/Config.hpp.in
    include/Volcano/Launcher/Config.hpp
)

set_target_properties(Launcher PROPERTIES OUTPUT_NAME VolcanoLauncher)

target_include_directories(Launcher PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Launcher)
target_link_libraries(Launcher PUBLIC Fusion)

find_package(argh REQUIRED)
target_link_libraries(Launcher PUBLIC argh)

find_package(SFML REQUIRED)
target_link_libraries(Launcher PUBLIC sfml::sfml)
