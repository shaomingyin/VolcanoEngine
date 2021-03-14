
include(FindGLFW3)
if(NOT GLFW3_FOUND)
    message(FATAL_ERROR "GLFW3 not found.")
endif()

volcano_executable(launcher
    OUTPUT_NAME
        Launcher
    DEPENDENCIES
        ui
        network
        game
        base
    PUBLIC_LINK_LIBRARIES
        ui
        network
        game
        base
        ${GLFW3_LIBRARIES}
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${GLFW3_INCLUDE_DIRS}
    PRIVATE_SOURCES
        include/Volcano/Launcher/Common.hpp
        src/Volcano/Launcher/Main.cpp
    )
