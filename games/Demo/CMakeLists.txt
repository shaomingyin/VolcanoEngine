
add_library(Demo STATIC
    include/Volcano/Demo/Common.h
    include/Volcano/Demo/Config.h.in
    include/Volcano/Demo/Game.h
    src/Volcano/Demo/Game.cpp
)

configure_file(
    include/Volcano/Demo/Config.h.in
    include/Volcano/Demo/Config.h
)

target_include_directories(Demo PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_game_setup(Demo)
target_link_libraries(Demo PUBLIC World)
target_link_libraries(Demo PUBLIC Physics)
set_target_properties(Demo PROPERTIES OUTPUT_NAME VolcanoDemo)
