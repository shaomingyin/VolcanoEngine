cmake_minimum_required(VERSION 3.13)

project(VolcanoEngine VERSION 0.1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug functions")
set(VOLCANO_EDITOR ON CACHE BOOL "Enable editor")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Concurrent REQUIRED)

add_library(Kernel SHARED)
target_compile_definitions(Kernel PRIVATE VOLCANO_KERNEL)
target_link_libraries(Kernel Qt5::Core Qt5::Gui)
set_target_properties(Kernel PROPERTIES OUTPUT_NAME "VolcanoKernel")

add_library(BulletPhysics SHARED)
target_compile_definitions(BulletPhysics PRIVATE VOLCANO_PHYSICS)
target_link_libraries(BulletPhysics Kernel Qt5Core)
set_target_properties(BulletPhysics PROPERTIES OUTPUT_NAME "VolcanoBulletPhysics")

add_library(OpenGLRenderer SHARED)
target_compile_definitions(OpenGLRenderer PRIVATE VOLCANO_OPENGLRENDERER)
target_link_libraries(OpenGLRenderer Kernel Qt5Core)
set_target_properties(OpenGLRenderer PROPERTIES OUTPUT_NAME "VolcanoOpenGLRenderer")

add_library(Editor SHARED)
target_compile_definitions(Editor PRIVATE VOLCANO_EDITOR)
target_link_libraries(Editor BulletPhysics OpenGLRenderer Kernel Qt5::Widgets)
set_target_properties(Editor PROPERTIES OUTPUT_NAME "VolcanoEditor")

add_subdirectory(include include)
add_subdirectory(src src)
