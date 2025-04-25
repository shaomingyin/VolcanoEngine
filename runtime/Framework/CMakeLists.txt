
volcano_add_static_library(Framework
    OUTPUT_NAME
        VolcanoFramework
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Framework/Config.h.in
        include/Volcano/Framework/Context.h
        include/Volcano/Framework/Common.h
        include/Volcano/Framework/Game.h
        src/Volcano/Framework/Context.cpp
        src/Volcano/Framework/Game.cpp
    PUBLIC_LINK_LIBRARIES
  		Gui
		Physics
		Graphics
		Acoustics
		World
        Schema
    PUBLIC_LINK_PACKAGES
        argh argh
    PUBLIC_HEADERS
        include/Volcano/Framework/Common.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Framework/Config.h
        DESTINATION include/Volcano/Framework
)
