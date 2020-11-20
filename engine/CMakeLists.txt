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

target_compile_definitions(engine PRIVATE
    DEBUG_DRAW_EXPLICIT_CONTEXT
    MICROPROFILE_USE_CONFIG
    )

conan_cmake_run(
    REQUIRES
        assimp/5.0.1
        bullet3/2.89
        sdl2/2.0.12@bincrafters/stable
    BASIC_SETUP
    CMAKE_TARGETS
    BUILD_TYPE "Release"
    BUILD missing
    )

target_link_libraries(engine
    CONAN_PKG::assimp
    CONAN_PKG::bullet3
    CONAN_PKG::sdl2
    )
