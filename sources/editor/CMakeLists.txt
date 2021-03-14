
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED Core Gui Widgets)

volcano_executable(editor
    OUTPUT_NAME
        Editor
    DEPENDENCIES
        ui
        network
        game
        base
    PUBLIC_LINK_LIBRARIES
        ui
        network
        game
        base
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Editor/Common.hpp
        include/Volcano/Editor/MainWindow.hpp
        include/Volcano/Editor/OutputView.hpp
        include/Volcano/Editor/Project.hpp
        include/Volcano/Editor/ProjectView.hpp
        include/Volcano/Editor/PropertyView.hpp
        include/Volcano/Editor/ResourceView.hpp
        include/Volcano/Editor/WorldView.hpp
        src/Volcano/Editor/Main.cpp
        src/Volcano/Editor/MainWindow.cpp
        src/Volcano/Editor/OutputView.cpp
        src/Volcano/Editor/Project.cpp
        src/Volcano/Editor/ProjectView.cpp
        src/Volcano/Editor/PropertyView.cpp
        src/Volcano/Editor/ResourceView.cpp
        src/Volcano/Editor/WorldView.cpp
    CONFIGURE_FILES
        include/Volcano/Editor/Config.hpp.in
    )

source_group(\\mocs REGULAR_EXPRESSION "mocs_.*\\.cpp")
