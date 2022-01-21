
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

add_definitions(-DWIN32_LEAN_AND_MEAN)

link_qt_modules(
	Concurrent
	Qml
	Gui
	OpenGL
	Quick
    )

if(WIN32)
    link_libraries(Winmm)
endif()

include(FindBullet)
if(BULLET_FOUND)
    include_directories(${BULLET_INCLUDE_DIRS})
    link_libraries(${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Bullet not found.")
endif()

file(GLOB_RECURSE RUNTIME_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.in
    )

qt_add_executable(Runtime ${RUNTIME_SOURCES})
set_target_properties(Runtime PROPERTIES OUTPUT_NAME "volcano")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RUNTIME_SOURCES})

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )
