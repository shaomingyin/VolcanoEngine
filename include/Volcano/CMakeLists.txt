
target_sources(Volcano
	PUBLIC
        Common.hpp
        Module.hpp
        ResourceManager.hpp
    PRIVATE
        Config.hpp.in
	)

configure_file(
    Config.hpp.in
    Config.hpp
    )

add_subdirectory(Game Game)
add_subdirectory(Graphics Graphics)
