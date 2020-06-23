
add_subdirectory(Renderer Renderer)

target_sources(Volcano
    PRIVATE
        Common.cpp
        Core.cpp
        Entity.cpp
        Object.cpp
        Scene.cpp
    )
