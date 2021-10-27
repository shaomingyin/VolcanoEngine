
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

link_libraries(Qt6::Concurrent Qt6::Gui Qt6::OpenGL Qt6::Quick)

include(FindBullet)
if(BULLET_FOUND)
    include_directories(${BULLET_INCLUDE_DIRS})
    link_libraries(${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Bullet not found.")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

file(GLOB_RECURSE RUNTIME_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

qt_add_executable(runtime ${RUNTIME_SOURCES})
set_target_properties(runtime PROPERTIES OUTPUT_NAME "volcano")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RUNTIME_SOURCES})

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )
