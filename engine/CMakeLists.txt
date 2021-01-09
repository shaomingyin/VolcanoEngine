#
#
#

###############################################################################
# common
###############################################################################

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(NodeJS)
nodejs_init()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

###############################################################################
# base
###############################################################################

file(GLOB_RECURSE VOLCANO_BASE_GLM_SOURCES LIST_DIRECTORIES false
    "include/glm/*.h"
    "include/glm/*.hpp"
    "include/glm/*.inc"
    )

set(VOLCANO_BASE_SOURCES
    include/argh.h

    include/sigslot/signal.hpp

    ${VOLCANO_BASE_GLM_SOURCES}

    include/debug_draw.hpp

    include/fontstash.h

    include/microprofile.config.h
    include/microprofile.h
    include/microprofile_html.h
    include/stb/stb_sprintf.h
    src/microprofile/microprofile.cpp

    include/fontstash.h
    include/nanovg.h
    include/nanovg_gl.h
    include/nanovg_gl_utils.h
    include/stb_image.h
    include/stb_truetype.h
    src/nanovg/nanovg.c

    include/oui.h

    include/GL/gl3w.h
    include/GL/glcorearb.h
    include/KHR/khrplatform.h
    src/GL/gl3w.c

    include/GL/Buffer.hpp
    include/GL/Common.hpp
    include/GL/FrameBuffer.hpp
    include/GL/Object.hpp
    include/GL/Program.hpp
    include/GL/Shader.hpp
    include/GL/Texture.hpp
    include/GL/VertexArray.hpp
    src/GL/Buffer.cpp
    src/GL/FrameBuffer.cpp
    src/GL/Object.cpp
    src/GL/Program.cpp
    src/GL/Shader.cpp
    src/GL/Texture.cpp
    src/GL/VertexArray.cpp

    include/Volcano/Color.hpp
    include/Volcano/Common.h
    include/Volcano/Common.hpp
    include/Volcano/Concurrent.hpp
    include/Volcano/Config.h.in
    include/Volcano/List.h
    include/Volcano/Point.hpp
    include/Volcano/Rect.hpp
    include/Volcano/ScopeGuard.hpp
    include/Volcano/Size.hpp
    include/Volcano/SpinLock.hpp
    src/Volcano/Common.cpp
    src/Volcano/List.c
    )

add_library(base STATIC ${VOLCANO_BASE_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_BASE_SOURCES})
set_target_properties(base PROPERTIES OUTPUT_NAME "VolcanoBase")

###############################################################################
# server
###############################################################################

set(VOLCANO_SERVER_SOURCES
    include/Volcano/Server/Camera.hpp
    include/Volcano/Server/Common.hpp
    include/Volcano/Server/Component.hpp
    include/Volcano/Server/DirectionalLight.hpp
    include/Volcano/Server/Entity.hpp
    include/Volcano/Server/Light.hpp
    include/Volcano/Server/Model.hpp
    include/Volcano/Server/Object.hpp
    include/Volcano/Server/PerpectiveCamera.hpp
    include/Volcano/Server/PointLight.hpp
    include/Volcano/Server/SpotLight.hpp
    include/Volcano/Server/World.hpp
    src/Volcano/Server/Camera.cpp
    src/Volcano/Server/Common.cpp
    src/Volcano/Server/Component.cpp
    src/Volcano/Server/Core.cpp
    src/Volcano/Server/DirectionalLight.cpp
    src/Volcano/Server/Entity.cpp
    src/Volcano/Server/Light.cpp
    src/Volcano/Server/Model.cpp
    src/Volcano/Server/Object.cpp
    src/Volcano/Server/PointLight.cpp
    src/Volcano/Server/SpotLight.cpp
    src/Volcano/Server/World.cpp
    )

add_nodejs_module(server ${VOLCANO_SERVER_SOURCES})
add_dependencies(server base)
target_link_libraries(server base)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_SERVER_SOURCES})
set_target_properties(server PROPERTIES OUTPUT_NAME "Volcano")

# BulletPhysics
#include(FindBullet)
#if(BULLET_FOUND)
#    target_include_directories(server PRIVATE ${BULLET_INCLUDE_DIRS})
#    target_link_libraries(server ${BULLET_LIBRARIES})
#else()
#    message(FATAL_ERROR "BulletPhysics not found.")
#endif()

###############################################################################
# client
###############################################################################

set(VOLCANO_CLIENT_SOURCES
    include/Volcano/Client/Common.hpp

    include/Volcano/Client/Graphics/Buffer.hpp
    include/Volcano/Client/Graphics/Common.hpp
    include/Volcano/Client/Graphics/DebugDrawRenderer.hpp
    include/Volcano/Client/Graphics/DirectionalLight.hpp
    include/Volcano/Client/Graphics/FrameBuffer.hpp
    include/Volcano/Client/Graphics/Heap.hpp
    include/Volcano/Client/Graphics/Light.hpp
    include/Volcano/Client/Graphics/Material.hpp
    include/Volcano/Client/Graphics/Memory.hpp
    include/Volcano/Client/Graphics/Mesh.hpp
    include/Volcano/Client/Graphics/PointLight.hpp
    include/Volcano/Client/Graphics/Renderer.hpp
    include/Volcano/Client/Graphics/SpotLight.hpp
    include/Volcano/Client/Graphics/View.hpp
    include/Volcano/Client/Graphics/Window.hpp
    src/Volcano/Client/Graphics/Buffer.cpp
    src/Volcano/Client/Graphics/DebugDrawRenderer.cpp
    src/Volcano/Client/Graphics/DirectionalLight.cpp
    src/Volcano/Client/Graphics/FrameBuffer.cpp
    src/Volcano/Client/Graphics/Heap.cpp
    src/Volcano/Client/Graphics/Light.cpp
    src/Volcano/Client/Graphics/Material.cpp
    src/Volcano/Client/Graphics/Memory.cpp
    src/Volcano/Client/Graphics/Mesh.cpp
    src/Volcano/Client/Graphics/PointLight.cpp
    src/Volcano/Client/Graphics/Renderer.cpp
    src/Volcano/Client/Graphics/SpotLight.cpp
    src/Volcano/Client/Graphics/View.cpp
    src/Volcano/Client/Graphics/Window.cpp

    include/Volcano/Client/Gui/Common.hpp
    include/Volcano/Client/Gui/Widget.hpp
    include/Volcano/Client/Gui/Window.hpp
    src/Volcano/Client/Gui/Widget.cpp
    src/Volcano/Client/Gui/Window.cpp

    include/Volcano/Client/Input/Common.hpp

    include/Volcano/Client/Sound/Common.hpp

    src/Volcano/Client/Main.cpp
    )

add_executable(client ${VOLCANO_CLIENT_SOURCES})
add_dependencies(client base)
target_link_libraries(client base)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_CLIENT_SOURCES})
set_target_properties(client PROPERTIES OUTPUT_NAME "Volcano")

# SDL2
include(FindSDL2)
if(SDL2_FOUND)
    target_include_directories(client PRIVATE ${SDL2_INCLUDE_DIR})
    target_link_libraries(client ${SDL2_LIBRARY})
else()
    message(FATAL_ERROR "SDL2 not found.")
endif()

# Assimp
#include(Find)
