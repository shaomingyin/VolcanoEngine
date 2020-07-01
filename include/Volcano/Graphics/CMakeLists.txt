
target_sources(Volcano
    PUBLIC
        Common.hpp
        Light.hpp
        Material.hpp
        Mesh.hpp
        Renderer.hpp
        View.hpp
    PRIVATE
        Config.hpp.in
    )

configure_file(
    Config.hpp.in
    Config.hpp
    )
