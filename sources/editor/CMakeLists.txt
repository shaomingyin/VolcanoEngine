
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED Core Gui Widgets)

add_executable(editor)

add_dependencies(editor
    ui
    network
    game
    base
    )

target_link_libraries(editor
    PUBLIC
        ui
        network
        game
        base
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
    )

target_include_directories(editor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_sources(editor
    PRIVATE
        include/Volcano/Editor/Common.hpp
        include/Volcano/Editor/Config.hpp.in
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

        src/Volcano/Editor/Resources/Main.qrc
    )

configure_file(
    include/Volcano/Editor/Config.hpp.in
    include/Volcano/Editor/Config.hpp
    )

source_group(\\qt_mocs FILES REGULAR_EXPRESSION "mocs_.*\.cpp$")

volcano_target_post_setup(editor)
