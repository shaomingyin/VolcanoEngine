
add_executable(Launcher
    include/Volcano/Launcher/Common.hpp
    include/Volcano/Launcher/Config.hpp.in
    src/Volcano/Launcher/Main.cpp
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
target_link_libraries(Launcher PUBLIC Framework)

find_package(argh REQUIRED)
target_link_libraries(Launcher PUBLIC argh)
