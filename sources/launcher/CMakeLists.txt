
add_executable(launcher WIN32)

add_dependencies(launcher
    ui
    network
    game
    base
    )

target_link_libraries(launcher
    PUBLIC
        ui
        network
        game
        base
    )

target_include_directories(launcher
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_sources(launcher   
    PRIVATE
        include/Volcano/Launcher/Common.hpp
        include/Volcano/Launcher/Context.hpp
        src/Volcano/Launcher/Context.cpp
        src/Volcano/Launcher/Main.cpp
    )

# SDL2
include(FindSDL2)
if(SDL2_FOUND)
    target_include_directories(launcher PUBLIC ${SDL2_INCLUDE_DIR})
    target_link_libraries(launcher PUBLIC ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})
else()
    message(FATAL_ERROR "SDL2 not found.")
endif()

volcano_target_post_setup(launcher)
