
volcano_add_static_library(System
    OUTPUT_NAME
        VolcanoSystem
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/System/Base.h
        include/Volcano/System/Client.h
        include/Volcano/System/Config.h.in
        include/Volcano/System/Common.h
        include/Volcano/System/Loader.h
        include/Volcano/System/Local.h
        include/Volcano/System/ScreenController.h
        include/Volcano/System/Server.h
        include/Volcano/System/Window.h
        src/Volcano/System/Base.cpp
        src/Volcano/System/Client.cpp
        src/Volcano/System/Loader.cpp
        src/Volcano/System/Local.cpp
        src/Volcano/System/ScreenController.cpp
        src/Volcano/System/Server.cpp
        src/Volcano/System/Window.cpp
    PUBLIC_LINK_LIBRARIES
        World
        Graphics
    PUBLIC_QT_MODULES
        Quick
    PUBLIC_HEADERS
        include/Volcano/System/Common.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/System/Config.h
        DESTINATION include/Volcano/System
)
