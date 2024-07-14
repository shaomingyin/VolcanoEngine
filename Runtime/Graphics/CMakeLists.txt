
find_package(VulkanHeaders REQUIRED)

volcano_glob_sources(GRAPHICS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
volcano_define_shared_library(Graphics
    OUTPUT_NAME
        VolcanoGraphics
    PRIVATE_SOURCES
        ${GRAPHICS_SOURCES}
    PRIVATE_DEFINITIONS
        VOLCANO_GRAPHICS
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PUBLIC_LINK_LIBRARIES
        Base
        vulkan-headers::vulkan-headers
    )
