
volcano_add_static_library(Player
    OUTPUT_NAME
        VolcanoPlayer
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/nlohmann_json.h

		include/Volcano/Player/Base.h
        include/Volcano/Player/Config.h.in
        include/Volcano/Player/Common.h
		include/Volcano/Player/Console.h
		include/Volcano/Player/Local.h
        include/Volcano/Player/Window.h
		src/Volcano/Player/Base.cpp
		src/Volcano/Player/Console.cpp
		src/Volcano/Player/Local.cpp
        src/Volcano/Player/Window.cpp
    PUBLIC_LINK_LIBRARIES
        Gui
	PUBLIC_LINK_PACKAGES
        nlohmann_json nlohmann_json::nlohmann_json
		SDL2 SDL2::SDL2
        PhysFS physfs-static
    PUBLIC_HEADERS
        include/Volcano/Player/Common.h
        include/Volcano/Player/Window.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Player/Config.h
        DESTINATION include/Volcano/Player
)
