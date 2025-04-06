
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
		Graphics
    PUBLIC_LINK_PACKAGES
        SDL3 SDL3::SDL3
    PUBLIC_HEADERS
        include/Volcano/Gui/Common.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Gui/Config.h
        DESTINATION include/Volcano/Gui
)
