
add_library(base STATIC)

target_compile_definitions(base
    PUBLIC
        NOMINMAX
    )

target_include_directories(base
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

file(GLOB_RECURSE SIGSLOT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sigslot/*
    )

file(GLOB_RECURSE RTTR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rttr/type
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rttr/registration
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rttr/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rttr/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rttr/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rttr/*.cpp
    )

file(GLOB_RECURSE EIGEN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Eigen/*
    )

target_sources(base
    PRIVATE
        ${ARGH_SOURCES}
        ${SIGSLOT_SOURCES}
        ${RTTR_SOURCES}
        ${EIGEN_SOURCES}
        ${MINIZ_SOURCES}
        include/argh.h
        include/Volcano/Buffer.hpp
        include/Volcano/Common.hpp
        include/Volcano/Config.hpp.in
        include/Volcano/IO.hpp
        include/Volcano/ScopeGuard.hpp
        include/Volcano/SpinLock.hpp
        src/Volcano/Buffer.cpp
        src/Volcano/Common.cpp
        src/Volcano/IO.cpp
    )

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )

# PhysFS
include(FindPhysFS)
if(PHYSFS_FOUND)
    target_include_directories(base PUBLIC ${PHYSFS_INCLUDE_DIR})
    target_link_libraries(base PUBLIC ${PHYSFS_LIBRARY})
else()
    message(FATAL_ERROR "PhysFS not found.")
endif()

volcano_target_post_setup(base)

