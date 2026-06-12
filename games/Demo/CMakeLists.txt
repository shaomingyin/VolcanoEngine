
add_library(Game STATIC
    include/Volcano/Demo/Common.h
    include/Volcano/Demo/Config.h.in
    include/Volcano/Demo/Game.h
    src/Volcano/Demo/Game.cpp
)

configure_file(
    include/Volcano/Demo/Config.h.in
    include/Volcano/Demo/Config.h
)

target_include_directories(Game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Game)
set_target_properties(Game PROPERTIES OUTPUT_NAME VolcanoGame)

target_link_libraries(Game PUBLIC Physics World)
