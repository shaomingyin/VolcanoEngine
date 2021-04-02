
# libuv
include(FindUV)
if(NOT UV_FOUND)
    message(FATAL_ERROR "libuv not found.")
endif()

volcano_executable(server
    DEPENDENCIES
        framework-base
    PRIVATE_LINK_LIBRARIES
        framework-base
        ${UV_LIBRARIES}
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${UV_INCLUDE_DIRS}
    PRIVATE_SOURCES
        include/Volcano/Server/Common.hpp
        src/Volcano/Server/Main.cpp
    )
