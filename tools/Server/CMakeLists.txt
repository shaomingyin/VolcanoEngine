
add_executable(Server
    include/Volcano/Server/Common.hpp
    include/Volcano/Server/Config.hpp.in
    src/Volcano/Server/Main.cpp
)

configure_file(
    include/Volcano/Server/Config.hpp.in
    include/Volcano/Server/Config.hpp
)

volcano_target_setup(Server)
set_target_properties(Server PROPERTIES OUTPUT_NAME VolcanoServer)

target_include_directories(Server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(Server PUBLIC World)

find_package(SFML REQUIRED COMPONENTS sfml-window)
target_link_libraries(Server PUBLIC sfml-window)

find_package(argh REQUIRED)
target_link_libraries(Server PUBLIC argh)
