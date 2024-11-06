
volcano_add_static_library(Gui
    OUTPUT_NAME
        VolcanoGui
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/portable-file-dialogs.h

        include/GL/gl3w.h
        include/GL/glcorearb.h
        include/KHR/khrplatform.h
        src/gl3w/gl3w.c
		
		include/imconfig.h
		include/imgui.h
		include/imgui_impl_opengl3.h
        include/imgui_impl_opengl3_loader.h
		include/imgui_impl_sdl2.h
		src/imgui/imgui.cpp
		src/imgui/imgui_demo.cpp
		src/imgui/imgui_draw.cpp
		src/imgui/imgui_internal.h
		src/imgui/imgui_tables.cpp
		src/imgui/imgui_widgets.cpp
		src/imgui/imstb_rectpack.h
		src/imgui/imstb_textedit.h
		src/imgui/imstb_truetype.h
		src/imgui/imgui_impl_opengl3.cpp
		src/imgui/imgui_impl_sdl2.cpp
		
        include/Volcano/Gui/Common.h
        include/Volcano/Gui/Config.h.in
        include/Volcano/Gui/Context.h
        include/Volcano/Gui/Point.h
        include/Volcano/Gui/Rect.h
        include/Volcano/Gui/Size.h
		include/Volcano/Gui/Widget.h
        include/Volcano/Gui/Window.h
        src/Volcano/Gui/Context.cpp
		src/Volcano/Gui/Widget.cpp
        src/Volcano/Gui/Window.cpp
    PUBLIC_LINK_LIBRARIES
        Base
        Sound
    PUBLIC_LINK_PACKAGES
        SDL2 SDL2::SDL2-static
    PUBLIC_HEADERS
        include/Volcano/Gui/Common.h
        include/Volcano/Gui/Widget.h
		include/Volcano/Gui/Window.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Gui/Config.h
        DESTINATION include/Volcano/Gui
)
