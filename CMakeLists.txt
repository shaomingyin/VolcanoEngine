cmake_minimum_required(VERSION 3.13)

project(VolcanoEngine VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/lib")

set(CMAKE_USE_RELATIVE_PATHS ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Base

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Base/include
    ${CMAKE_CURRENT_BINARY_DIR}/Base/include
    )

add_subdirectory(Base Base)

# BulletPhysics

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/BulletPhysics/include
    ${CMAKE_CURRENT_BINARY_DIR}/BulletPhysics/include
    )

add_subdirectory(BulletPhysics BulletPhysics)

# OpenGLRenderer

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLRenderer/include
    ${CMAKE_CURRENT_BINARY_DIR}/OpenGLRenderer/include
    )

add_subdirectory(OpenGLRenderer OpenGLRenderer)

# VulkanRenderer

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/VulkanRenderer/include
    ${CMAKE_CURRENT_BINARY_DIR}/VulkanRenderer/include
    )

add_subdirectory(VulkanRenderer VulkanRenderer)

# Editor

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Editor/include
    ${CMAKE_CURRENT_BINARY_DIR}/Editor/include
    )

add_subdirectory(Editor Editor)

# Templates

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Templates/include
    ${CMAKE_CURRENT_BINARY_DIR}/Templates/include
    )

add_subdirectory(Templates Templates)

# Others

add_subdirectory(Demo Demo)
add_subdirectory(Tools Tools)
add_subdirectory(Documents Documents)
