
volcano_add_static_library(Editor
    OUTPUT_NAME
        VolcanoEditor
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
	    include/debug_draw.hpp
        src/debug_draw/debug_draw.cpp
        include/Volcano/Editor/Config.h.in
        include/Volcano/Editor/Common.h
        src/Volcano/Editor/Common.cpp
    PUBLIC_LINK_LIBRARIES
		Gui
)
