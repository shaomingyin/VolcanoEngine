
volcano_add_static_library(System
    OUTPUT_NAME
        VolcanoSystem
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/nlohmann_json.h
		
		include/imconfig.h
		include/imgui.h
		include/imgui_impl_opengl3.h
		include/imgui_impl_opengl3_loader.h
		include/imgui_impl_sdl2.h
		src/imgui/imgui.cpp
		src/imgui/imgui_demo.cpp
		src/imgui/imgui_draw.cpp
		src/imgui/imgui_impl_opengl3.cpp
		src/imgui/imgui_impl_sdl2.cpp
		src/imgui/imgui_internal.h
		src/imgui/imgui_tables.cpp
		src/imgui/imgui_widgets.cpp
		src/imgui/imstb_rectpack.h
		src/imgui/imstb_textedit.h
		src/imgui/imstb_truetype.h

		include/Volcano/System/Base.h
		include/Volcano/System/Client.h
        include/Volcano/System/Config.h.in
        include/Volcano/System/Common.h
		include/Volcano/System/Console.h
		include/Volcano/System/Local.h
		include/Volcano/System/Server.h
		include/Volcano/System/Stepper.h
		include/Volcano/System/Window.h
		src/Volcano/System/Base.cpp
		src/Volcano/System/Client.cpp
		src/Volcano/System/Console.cpp
		src/Volcano/System/Local.cpp
		src/Volcano/System/Server.cpp
		src/Volcano/System/Window.cpp
    PUBLIC_LINK_LIBRARIES
		World
        Graphics
		Acoustics
	PUBLIC_LINK_PACKAGES
        nlohmann_json nlohmann_json::nlohmann_json
        enet enet::enet
        PhysFS physfs-static
		SDL2 SDL2::SDL2
    PUBLIC_HEADERS
        include/Volcano/System/Common.h
        include/Volcano/System/Window.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/System/Config.h
        DESTINATION include/Volcano/System
)
