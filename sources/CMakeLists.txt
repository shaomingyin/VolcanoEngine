
# libuv
include(FindUV)
if(UV_FOUND)
    include_directories(${UV_INCLUDE_DIRS})
    link_libraries(${UV_LIBRARIES})
else()
    message(FATAL_ERROR "libuv not found.")
endif()

# PhysFS
include(FindPhysFS)
if(PHYSFS_FOUND)
    include_directories(${PHYSFS_INCLUDE_DIR})
    link_libraries(${PHYSFS_LIBRARY})
else()
    message(FATAL_ERROR "PhysFS not found.")
endif()

# SDL2
include(FindSDL2)
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
    link_libraries(${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})
else()
    message(FATAL_ERROR "SDL2 not found.")
endif()

# ByteOrder
include(TestBigEndian)
TEST_BIG_ENDIAN(VOLCANO_BIG_ENDIAN)

configure_file(
    include/Volcano/Config.h.in
    include/Volcano/Config.h
    )

file(GLOB_RECURSE VOLCANO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_definitions(
    -DNOMINMAX
    -DLUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/include/Volcano/VM/Task.h\"
    )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

add_executable(volcano ${VOLCANO_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_SOURCES})
