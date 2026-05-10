
add_library(Stage STATIC
    include/Volcano/Stage/Appearance.h
    include/Volcano/Stage/Camera.h
    include/Volcano/Stage/Common.h
    include/Volcano/Stage/Config.h.in
    include/Volcano/Stage/Collision.h
    include/Volcano/Stage/Inherent.h
    include/Volcano/Stage/Lighting.h
    include/Volcano/Stage/Map.h
    include/Volcano/Stage/Scene.h
    include/Volcano/Stage/Screen.h
    include/Volcano/Stage/Sound.h
    src/Volcano/Stage/Common.cpp
    src/Volcano/Stage/Scene.cpp
)

configure_file(
    include/Volcano/Stage/Config.h.in
    include/Volcano/Stage/Config.h
)

target_include_directories(Stage PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Stage)
set_target_properties(Stage PROPERTIES OUTPUT_NAME VolcanoStage)

target_link_libraries(Stage PUBLIC Base)

find_package(EnTT)
target_link_libraries(Stage PUBLIC EnTT::EnTT)
