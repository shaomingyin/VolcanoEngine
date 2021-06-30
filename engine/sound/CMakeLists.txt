
file(GLOB_RECURSE SOUND_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_library(sound STATIC ${SOUND_SOURCES})
add_dependencies(sound game)
target_link_libraries(sound PUBLIC game)
set_target_properties(sound PROPERTIES OUTPUT_NAME "VolcanoSound")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOUND_SOURCES})

target_include_directories(sound
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

configure_file(
    include/Volcano/Sound/Config.hpp.in
    include/Volcano/Sound/Config.hpp
    )

#find_package(Qt6 COMPONENTS Concurrent Gui OpenGL Widgets Qml Quick REQUIRED)
#link_libraries(Qt6::Concurrent Qt6::Gui Qt6::OpenGL Qt6::Widgets Qt6::Qml Qt6::Quick)
