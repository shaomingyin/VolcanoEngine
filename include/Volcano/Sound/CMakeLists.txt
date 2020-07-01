
target_sources(Volcano
    PUBLIC
        Common.hpp
    PRIVATE
        Config.hpp.in
    )

configure_file(
    Config.hpp.in
    Config.hpp
    )
