
# NodeJS
include(NodeJS)
set(NODEJS_DEFAULT_URL https://nodejs.org/download/release CACHE STRING "NodeJS release url.")
nodejs_init()

# GLFW
include(FindGLFW)
if(GLFW_FOUND)
    include_directories(${GLFW_INCLUDE_DIRS})
    link_libraries(${GLFW_LIBRARIES})
else()
    message(FATAL_ERROR "GLFW not found.")
endif()

# BulletPhysics
include(FindBullet)
if(BULLET_FOUND)
    link_libraries(${BULLET_LIBRARIRES})
    include_directories(${BULLET_INCLUDE_DIR})
else()
    message(FATAL_ERROR "BulletPhysics not found.")
endif()

add_definitions(
    -DNOMAXMIN
    -DLIBASYNC_STATIC
    -DMICROPROFILE_GPU_TIMERS_GL
    -DMICROPROFILE_USE_CONFIG=1
    )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )

file(GLOB_RECURSE VOLCANO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_nodejs_module(engine ${VOLCANO_SOURCES})
set_target_properties(engine PROPERTIES OUTPUT_NAME "volcano-engine")

source_group(\\_node FILES ${NODEJS_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_SOURCES})

target_compile_features(engine
    PRIVATE
        cxx_constexpr
        cxx_thread_local
        cxx_std_17
    )
