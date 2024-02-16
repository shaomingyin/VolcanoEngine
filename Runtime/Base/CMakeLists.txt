
volcano_glob_sources(BASE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
volcano_define_static_library(Base
    OUTPUT_NAME
        VolcanoBase
    PRIVATE_SOURCES
        ${BASE_SOURCES}
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PUBLIC_QT_MODULES
        Concurrent
        Gui
        Qml
    )
