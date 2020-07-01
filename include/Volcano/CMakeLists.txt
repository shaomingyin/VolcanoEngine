
add_subdirectory(Graphics Graphics)
add_subdirectory(Physics Physics)
add_subdirectory(Sound Sound)

target_sources(Volcano
    PUBLIC
        Camera.hpp
        Common.hpp
        Entity.hpp
        Event.hpp
        Graphics.hpp
        Node.hpp
        Physics.hpp
        Scene.hpp
        Sound.hpp
    PRIVATE
        Config.hpp.in
    )
