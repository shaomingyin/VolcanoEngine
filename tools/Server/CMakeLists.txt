
add_executable(Server
    include/Volcano/Server/Common.h
    include/Volcano/Server/Config.h.in
    src/Volcano/Server/Main.cpp
)

configure_file(
    include/Volcano/Server/Config.h.in
    include/Volcano/Server/Config.h
)

volcano_target_setup(Server)
set_target_properties(Server PROPERTIES OUTPUT_NAME VolcanoServer)

target_include_directories(Server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(Server PUBLIC Base)

find_package(SFML REQUIRED COMPONENTS Window)
target_link_libraries(Server PUBLIC SFML::Window)

find_package(argh REQUIRED)
target_link_libraries(Server PUBLIC argh)
