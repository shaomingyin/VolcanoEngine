
volcano_add_static_library(Gui
    OUTPUT_NAME
        VolcanoGui
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Gui/Config.h.in
        include/Volcano/Gui/Common.h
        src/Volcano/Gui/Common.cpp
	PUBLIC_LINK_LIBRARIES
        Acoustics
		Graphics
)
