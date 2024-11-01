
volcano_add_static_library(Gui
    OUTPUT_NAME
        VolcanoGui
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/portable-file-dialogs.h
        include/Volcano/Gui/Common.h
        include/Volcano/Gui/Config.h.in
        include/Volcano/Gui/Context.h
		include/Volcano/Gui/Widget.h
        include/Volcano/Gui/Window.h
        src/Volcano/Gui/Context.cpp
		src/Volcano/Gui/Widget.cpp
        src/Volcano/Gui/Window.cpp
    PUBLIC_LINK_LIBRARIES
        Graphics
        Sound
    PUBLIC_LINK_PACKAGES
        SDL2 SDL2::SDL2
    PUBLIC_HEADERS
        include/Volcano/Gui/Common.h
        include/Volcano/Gui/Widget.h
		include/Volcano/Gui/Window.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Gui/Config.h
        DESTINATION include/Volcano/Gui
)
