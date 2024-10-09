
volcano_add_shared_library(Graphics
    OUTPUT_NAME
        VolcanoGraphics
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Graphics/Config.h.in
        include/Volcano/Graphics/Common.h
        include/Volcano/Graphics/Driver.h
        include/Volcano/Graphics/Renderer.h
        include/Volcano/Graphics/View.h
        src/Volcano/Graphics/Driver.cpp
        src/Volcano/Graphics/Renderer.cpp
        src/Volcano/Graphics/View.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_HEADERS
        include/Volcano/Graphics/Common.h
        include/Volcano/Graphics/Driver.h
        include/Volcano/Graphics/Renderer.h
        include/Volcano/Graphics/View.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Graphics/Config.h
        DESTINATION include/Volcano/Graphics
)
