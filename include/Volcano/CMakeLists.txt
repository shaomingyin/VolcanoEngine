
target_sources(Volcano
    PUBLIC
        Common.hpp
        Config.hpp.in
        Drivers.hpp
        Gui.hpp
        Module.hpp
        Resource.hpp
        Scene.hpp
        VM.hpp
    )

configure_file(
    Config.hpp.in
    Config.hpp
    )

add_subdirectory(Drivers Drivers)
add_subdirectory(Gui Gui)
add_subdirectory(Scene Scene)
