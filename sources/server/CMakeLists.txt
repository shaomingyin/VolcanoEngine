
add_executable(server)

add_dependencies(server
    network
    game
    base
    )

target_link_libraries(server
    PRIVATE
        network
        game
        base
    )

target_include_directories(server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_sources(server
    PRIVATE
        include/Volcano/Server/Common.hpp
        src/Volcano/Server/Main.cpp
    )

# libuv
include(FindUV)
if(UV_FOUND)
    target_include_directories(server PUBLIC ${UV_INCLUDE_DIRS})
    target_link_libraries(server PUBLIC ${UV_LIBRARIES})
else()
    message(FATAL_ERROR "libuv not found.")
endif()

volcano_target_post_setup(server)

