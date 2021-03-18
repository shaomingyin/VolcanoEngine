
add_executable(launcher)

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
        src/Volcano/Launcher/Main.cpp
    )

# GLFW3
include(FindGLFW3)
if(GLFW3_FOUND)
    target_include_directories(launcher PUBLIC ${GLFW3_INCLUDE_DIRS})
    target_link_libraries(launcher PUBLIC ${GLFW3_LIBRARIES})
else()
    message(FATAL_ERROR "GLFW3 not found.")
endif()

volcano_target_post_setup(launcher)

