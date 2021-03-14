
include(FindPhysFS)
if(NOT PHYSFS_FOUND)
    message(FATAL_ERROR "PhysFS not found.")
endif()

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

set(ARGH_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/argh.h
    )

volcano_library(base
    OUTPUT_NAME
        Base
    PUBLIC_DEFINITIONS
        NOMINMAX
    PUBLIC_INCLUDE_DIRECTORIES
        ${PHYSFS_INCLUDE_DIR}
        ${PHYSFS_LIBRARY}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        ${ARGH_SOURCES}
        ${SIGSLOT_SOURCES}
        ${RTTR_SOURCES}
        ${EIGEN_SOURCES}
        ${MINIZ_SOURCES}
        include/Volcano/Buffer.hpp
        include/Volcano/Common.hpp
        include/Volcano/FileSystem.hpp
        include/Volcano/IO.hpp
        include/Volcano/ScopeGuard.hpp
        include/Volcano/SpinLock.hpp
        src/Volcano/Buffer.cpp
        src/Volcano/Common.cpp
        src/Volcano/IO.cpp
    CONFIGURE_FILES
        include/Volcano/Config.hpp.in
    )
