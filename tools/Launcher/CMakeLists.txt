
volcano_add_static_library(Launcher
    OUTPUT_NAME
        VolcanoLauncher
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Launcher/Common.h
        include/Volcano/Launcher/Config.h.in
        include/Volcano/Launcher/MainWindow.h
        src/Volcano/Launcher/Main.cpp
        src/Volcano/Launcher/MainWindow.cpp
    PUBLIC_LINK_LIBRARIES
		Acoustics
		Graphics
		Physics
        World
    PUBLIC_HEADERS
        include/Volcano/Launcher/Common.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Launcher/Config.h
        DESTINATION include/Volcano/Launcher
)
