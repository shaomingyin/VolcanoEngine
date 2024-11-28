
volcano_add_auto_executable(Demo
    OUTPUT_NAME
        VolcanoDemo
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Demo/Application.h
        include/Volcano/Demo/Common.h
        src/Volcano/Demo/Application.cpp
        src/Volcano/Demo/Main.cpp
    PUBLIC_LINK_LIBRARIES
        Player
		Acoustics
		Graphics
    PUBLIC_LINK_PACKAGES
        argh argh
        SDL2 SDL2::SDL2main
)
