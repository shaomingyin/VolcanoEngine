
volcano_add_auto_executable(Demo
    OUTPUT_NAME
        VolcanoDemo
    PRIVATE_SOURCES
        Main.cpp
    PUBLIC_LINK_LIBRARIES
        System
    PUBLIC_LINK_PACKAGES
        argh argh
        SDL2 SDL2::SDL2main
)
