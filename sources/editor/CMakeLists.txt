
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED Core Gui Widgets)

file(GLOB_RECURSE EDITOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_executable(editor
    DEPENDENCIES
        drivers
        framework
    PUBLIC_LINK_LIBRARIES
        drivers
        framework
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        ${EDITOR_SOURCES}
    )

configure_file(
    include/Volcano/Editor/Config.hpp.in
    include/Volcano/Editor/Config.hpp
    )

source_group(\\qt_mocs FILES REGULAR_EXPRESSION "mocs_.*\.cpp$")
