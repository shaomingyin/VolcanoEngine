#
#
#

cmake_minimum_required(VERSION 3.15)

project(VolcanoEngine VERSION 0.1.0)

if(MSVC)
    add_definitions(-D _CRT_SECURE_NO_WARNINGS)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake")

include(CMakeDependentOption)

option(VOLCANO_DEBUG "Enable debug funtions." ON)
option(VOLCANO_OUT_DIR "Build output directory." "${CMAKE_BINARY_DIR}/out")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${VOLCANO_OUT_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${VOLCANO_OUT_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${VOLCANO_OUT_DIR}/lib")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

macro(link_qt_modules)
    foreach(QT_MODULE ${ARGN})
        find_package(Qt6 COMPONENTS ${QT_MODULE} REQUIRED)
        link_libraries(Qt6::${QT_MODULE})
    endforeach()
endmacro()

macro(target_link_qt_modules TARGET SCOPE)
    foreach(QT_MODULE ${ARGN})
        find_package(Qt6 COMPONENTS ${QT_MODULE} REQUIRED)
        target_link_libraries(${TARGET} ${SCOPE} Qt6::${QT_MODULE})
    endforeach()
endmacro()

link_qt_modules(Core)

add_subdirectory(Runtime Runtime)
add_subdirectory(Templates Templates)
add_subdirectory(Packages Packages)
