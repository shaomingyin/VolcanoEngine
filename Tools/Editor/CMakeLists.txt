
volcano_glob_sources(EDITOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
volcano_define_executable(Editor
    OUTPUT_NAME
        VolcanoEditor
    PRIVATE_SOURCES
        ${EDITOR_SOURCES}
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_QT_MODULES
        Widgets
    )
