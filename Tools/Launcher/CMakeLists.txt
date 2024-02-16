
volcano_glob_sources(LAUNCHER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
volcano_define_executable(Launcher
    OUTPUT_NAME
        VolcanoLauncher
    PRIVATE_SOURCES
        ${LAUNCHER_SOURCES}
    PRIVATE_LINK_LIBRARIES
        Graphics
        Base
    PRIVATE_QT_MODULES
        Qml
        Quick
    )
