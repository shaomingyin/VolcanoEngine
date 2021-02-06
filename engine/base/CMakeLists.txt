
find_package(Qt6 COMPONENTS Concurrent REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS OpenGL REQUIRED)

add_library(base SHARED)

set_target_properties(base PROPERTIES OUTPUT_NAME "Volcano")

target_compile_definitions(base
    PRIVATE
        VOLCANO
    )

target_include_directories(base
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_link_libraries(base
    PUBLIC
        Qt6::Concurrent
        Qt6::Gui
        Qt6::OpenGL
    )

target_sources(base
    PUBLIC
        include/Volcano/Common.hpp
        include/Volcano/Context.hpp
        include/Volcano/Plugin.hpp
        include/Volcano/PluginManager.hpp
        include/Volcano/Game/Camera.hpp
        include/Volcano/Game/Common.hpp
        include/Volcano/Game/Component.hpp
        include/Volcano/Game/DirectionalLight.hpp
        include/Volcano/Game/Entity.hpp
        include/Volcano/Game/Light.hpp
        include/Volcano/Game/Model.hpp
        include/Volcano/Game/PerpectiveCamera.hpp
        include/Volcano/Game/PointLight.hpp
        include/Volcano/Game/SpotLight.hpp
        include/Volcano/Game/World.hpp
        include/Volcano/Graphics/Buffer.hpp
        include/Volcano/Graphics/Common.hpp
        include/Volcano/Graphics/DirectionalLight.hpp
        include/Volcano/Graphics/Heap.hpp
        include/Volcano/Graphics/Light.hpp
        include/Volcano/Graphics/Material.hpp
        include/Volcano/Graphics/Memory.hpp
        include/Volcano/Graphics/Mesh.hpp
        include/Volcano/Graphics/PointLight.hpp
        include/Volcano/Graphics/Renderer.hpp
        include/Volcano/Graphics/SpotLight.hpp
        include/Volcano/Graphics/View.hpp
        include/Volcano/Input/Common.hpp
        include/Volcano/Sound/Common.hpp
    PRIVATE
        include/Volcano/Config.hpp.in
        src/Volcano/Common.cpp
        src/Volcano/Context.cpp
        src/Volcano/Plugin.cpp
        src/Volcano/PluginManager.cpp
        src/Volcano/Game/Camera.cpp
        src/Volcano/Game/Common.cpp
        src/Volcano/Game/Component.cpp
        src/Volcano/Game/DirectionalLight.cpp
        src/Volcano/Game/Entity.cpp
        src/Volcano/Game/Light.cpp
        src/Volcano/Game/Model.cpp
        src/Volcano/Game/PointLight.cpp
        src/Volcano/Game/SpotLight.cpp
        src/Volcano/Game/World.cpp
        src/Volcano/Graphics/Buffer.cpp
        src/Volcano/Graphics/DirectionalLight.cpp
        src/Volcano/Graphics/Heap.cpp
        src/Volcano/Graphics/Light.cpp
        src/Volcano/Graphics/Material.cpp
        src/Volcano/Graphics/Memory.cpp
        src/Volcano/Graphics/Mesh.cpp
        src/Volcano/Graphics/PointLight.cpp
        src/Volcano/Graphics/Renderer.cpp
        src/Volcano/Graphics/SpotLight.cpp
        src/Volcano/Graphics/View.cpp
    )

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )

get_target_property(BASE_SOURCES base SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${BASE_SOURCES})
