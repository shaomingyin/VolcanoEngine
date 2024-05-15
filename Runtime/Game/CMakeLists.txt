
volcano_glob_sources(GAME_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
volcano_define_shared_library(Game
    OUTPUT_NAME
        VolcanoGame
    PRIVATE_SOURCES
        ${GAME_SOURCES}
    PRIVATE_DEFINITIONS
        VOLCANO_GAME
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PUBLIC_LINK_LIBRARIES
        Physics
        Graphics
        Base
    PUBLIC_QT_MODULES
        Qml
    )
