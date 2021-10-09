
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(FindBullet)
if(BULLET_FOUND)
    include_directories(${BULLET_INCLUDE_DIRS})
    link_libraries(${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Bullet not found.")
endif()

macro(link_qt6)
    foreach(COMPONENT ${ARGN})
        find_package(Qt6 COMPONENTS ${COMPONENT} REQUIRED)
        link_libraries(Qt6::${COMPONENT})
    endforeach()
endmacro()

link_qt6(Core Concurrent Gui Qml Quick)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

file(GLOB_RECURSE ENGINE_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_executable(engine ${ENGINE_SOURCES})
set_target_properties(engine PROPERTIES OUTPUT_NAME "volcano")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCES})

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )
