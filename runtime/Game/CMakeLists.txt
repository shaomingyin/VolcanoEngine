
volcano_add_shared_library(Game
    OUTPUT_NAME
        VolcanoGame
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Game/Config.h.in
        include/Volcano/Game/Common.h
        include/Volcano/Game/Object.h
        include/Volcano/Game/World.h
        src/Volcano/Game/Object.cpp
        src/Volcano/Game/World.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_HEADERS
        include/Volcano/Game/Common.h
        include/Volcano/Game/Object.h
        include/Volcano/Game/World.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Game/Config.h
        DESTINATION include/Volcano/Game
)
