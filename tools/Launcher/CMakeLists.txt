
volcano_add_executable(Launcher
    OUTPUT_NAME
        VolcanoLauncher
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Launcher/Config.h.in
        include/Volcano/Launcher/Common.h
        include/Volcano/Launcher/MainWindow.h
        src/Volcano/Launcher/MainWindow.cpp
        src/Volcano/Launcher/Main.cpp
    PUBLIC_LINK_LIBRARIES
        Game
        OpenGL
        Bullet
        Sound
    PUBLIC_LINK_PACKAGES
        argh argh
        glfw3 glfw
)
