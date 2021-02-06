
find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(editor)

add_dependencies(editor launcher base)
set_target_properties(editor PROPERTIES OUTPUT_NAME "VolcanoEditor")

target_include_directories(editor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_link_libraries(editor
    PRIVATE
        base
        Qt6::Widgets
    )

target_sources(editor
    PRIVATE
        include/Volcano/Editor/Common.hpp
        include/Volcano/Editor/MainWindow.hpp
        include/Volcano/Editor/OutputView.hpp
        include/Volcano/Editor/Project.hpp
        include/Volcano/Editor/ProjectView.hpp
        include/Volcano/Editor/PropertyView.hpp
        include/Volcano/Editor/ResourceView.hpp
        include/Volcano/Editor/SceneView.hpp
        src/Volcano/Editor/Main.cpp
        src/Volcano/Editor/MainWindow.cpp
        src/Volcano/Editor/OutputView.cpp
        src/Volcano/Editor/Project.cpp
        src/Volcano/Editor/ProjectView.cpp
        src/Volcano/Editor/PropertyView.cpp
        src/Volcano/Editor/ResourceView.cpp
        src/Volcano/Editor/SceneView.cpp
    )

get_target_property(EDITOR_SOURCES editor SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCES})
