#
#
#

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(engine)

target_include_directories(engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

file(GLOB_RECURSE VOLCANO_INCLUDES "*.h" "*.hpp")
file(GLOB_RECURSE VOLCANO_SOURCES "*.c" "*.cpp")
file(GLOB_RECURSE VOLCANO_CONFIGS "*.in")

set(VOLCANO_ALL_SOURCES ${VOLCANO_INCLUDES} ${VOLCANO_SOURCES} ${VOLCANO_CONFIGS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_ALL_SOURCES})
target_sources(engine PRIVATE ${VOLCANO_ALL_SOURCES})
set_target_properties(engine PROPERTIES OUTPUT_NAME "volcano")

target_compile_definitions(engine PRIVATE
    LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/include/Volcano/VM/Task.h\"
    )

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug functions.")

foreach(CONFIG_PATH ${VOLCANO_CONFIGS})
    file(RELATIVE_PATH CONFIG_RPATH ${CMAKE_CURRENT_SOURCE_DIR} ${CONFIG_PATH})
    string(REGEX REPLACE "\\.in\$" "" CONFIG_OUTPUT_RPATH ${CONFIG_RPATH})
    configure_file(${CONFIG_RPATH} ${CONFIG_OUTPUT_RPATH})
endforeach()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    target_link_libraries(engine stdc++fs)
endif()

if(UNIX)
    target_link_libraries(engine dl m)
endif()

target_compile_definitions(engine PRIVATE
    DEBUG_DRAW_EXPLICIT_CONTEXT
    MICROPROFILE_USE_CONFIG
    )

# libuv
include(FindUV)
if(UV_FOUND)
    target_include_directories(engine PRIVATE ${UV_INCLUDE_DIRS})
    target_link_libraries(engine ${UV_LIBRARIES})
else()
    message(FATAL_ERROR "libuv not found.")
endif()

# SDL2
include(FindSDL2)
if(SDL2_FOUND)
    target_include_directories(engine PRIVATE ${SDL2_INCLUDE_DIR})
    target_link_libraries(engine ${SDL2_LIBRARY})
else()
    message(FATAL_ERROR "SDL2 not found.")
endif()

# BulletPhysics
include(FindBullet)
if(BULLET_FOUND)
    target_include_directories(engine PRIVATE ${BULLET_INCLUDE_DIRS})
    target_link_libraries(engine ${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "BulletPhysics not found.")
endif()

# Assimp
include(FindAssimp)
if(ASSIMP_FOUND)
    target_include_directories(engine PRIVATE ${ASSIMP_INCLUDE_DIR})
    target_link_libraries(engine ${ASSIMP_LIBRARIES})
else()
    message(FATAL_ERROR "Assimp not found.")
endif()
