
volcano_add_executable(Launcher
    OUTPUT_NAME
        VolcanoLauncher
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Launcher/Config.h.in
        include/Volcano/Launcher/Common.h
        include/Volcano/Launcher/Application.h
        src/Volcano/Launcher/Application.cpp
        src/Volcano/Launcher/Main.cpp
    PUBLIC_LINK_LIBRARIES
        System
    PUBLIC_LINK_PACKAGES
        argh argh
)
