
# SDL2
include(FindSDL2)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found.")
endif()

volcano_executable(launcher
    DEPENDENCIES
        drivers
        framework-base
    PRIVATE_LINK_LIBRARIES
        drivers
        framework-base
        ${SDL2_LIBRARY}
        ${SDL2MAIN_LIBRARY}
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${SDL2_INCLUDE_DIR}
    PRIVATE_SOURCES
        include/Volcano/Launcher/Application.hpp
        include/Volcano/Launcher/Common.hpp
        src/Volcano/Launcher/Application.cpp
        src/Volcano/Launcher/Main.cpp
    )
