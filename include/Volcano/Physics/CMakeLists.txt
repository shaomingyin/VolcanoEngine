
target_sources(Volcano
    PUBLIC
        Box.hpp
        Common.hpp
        RigidBody.hpp
        World.hpp
    PRIVATE
        Config.hpp.in
    )

configure_file(
    Config.hpp.in
    Config.hpp
    )
