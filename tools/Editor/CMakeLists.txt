
volcano_add_executable(Editor
    OUTPUT_NAME
        VolcanoEditor
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Editor/Config.h.in
        include/Volcano/Editor/Common.h
        src/Volcano/Editor/Main.cpp
	DEPENDENCIES
		Launcher
		Server
    PUBLIC_LINK_LIBRARIES
		Framework
)
