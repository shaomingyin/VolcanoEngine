
add_library(network STATIC)

add_dependencies(network
    game
    base
    )

target_link_libraries(network
    PUBLIC
        game
        base
    )

target_include_directories(network
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_sources(network
    PRIVATE
        include/Volcano/Network/Common.hpp
        include/Volcano/Network/Config.hpp.in
        src/Volcano/Network/Common.cpp
    )

configure_file(
    include/Volcano/Network/Config.hpp.in
    include/Volcano/Network/Config.hpp
    )

volcano_target_post_setup(network)

