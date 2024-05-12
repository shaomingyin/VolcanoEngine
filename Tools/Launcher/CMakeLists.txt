
set(VOLCANO_LAUNCHER ON CACHE BOOL "Enable launcher.")
if(NOT VOLCANO_LAUNCHER)
    return()
endif()

volcano_glob_sources(LAUNCHER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
volcano_define_executable(Launcher
    OUTPUT_NAME
        VolcanoLauncher
    PRIVATE_SOURCES
        ${LAUNCHER_SOURCES}
    PRIVATE_LINK_LIBRARIES
        Game
    PRIVATE_QT_MODULES
        Qml
        Quick
    )
