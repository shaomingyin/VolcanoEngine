
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Concurrent REQUIRED)

add_library(Editor SHARED)

add_dependencies(Editor
    BulletPhysics
    OpenGLRenderer
    Base
    )

target_compile_definitions(Editor
    PRIVATE
        VOLCANO_EDITOR
    )

set_target_properties(Editor
    PROPERTIES
        OUTPUT_NAME "VolcanoEditor"
    )

target_link_libraries(Editor
    BulletPhysics
    OpenGLRenderer
    Base
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Test
    Qt5::Concurrent
    )

add_subdirectory(include include)
add_subdirectory(src src)
