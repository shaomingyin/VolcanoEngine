
find_package(Bullet)

volcano_glob_sources(RUNTIME_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
volcano_define_shared_library(Runtime
    OUTPUT_NAME
        VolcanoRuntime
    PRIVATE_SOURCES
        ${RUNTIME_SOURCES}
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${BULLET_INCLUDE_DIRS}
    PUBLIC_LINK_LIBRARIES
        Bullet::Bullet
    PUBLIC_QT_MODULES
        Concurrent
        Gui
        Qml
    )
