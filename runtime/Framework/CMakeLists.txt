
add_library(Framework STATIC
    include/Volcano/Framework/Base.h
    include/Volcano/Framework/Common.h
    include/Volcano/Framework/Config.h.in
    include/Volcano/Framework/Context.h
    include/Volcano/Framework/Local.h
    include/Volcano/Framework/Client.h
    include/Volcano/Framework/Server.h
    include/Volcano/Framework/Window.h
    src/Volcano/Framework/Base.cpp
    src/Volcano/Framework/Context.cpp
    src/Volcano/Framework/Local.cpp
    src/Volcano/Framework/Client.cpp
    src/Volcano/Framework/Server.cpp
    src/Volcano/Framework/Window.cpp
)

configure_file(
    include/Volcano/Framework/Config.h.in
    include/Volcano/Framework/Config.h
)

target_include_directories(Framework PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Framework)
set_target_properties(Framework PROPERTIES OUTPUT_NAME VolcanoFramework)

target_link_libraries(Framework PUBLIC World Graphics Gui)

find_package(Async++)
target_link_libraries(Framework PUBLIC Async++)

find_package(nlohmann_json)
target_link_libraries(Framework PUBLIC nlohmann_json::nlohmann_json)

find_package(PhysFS)
target_link_libraries(Framework PUBLIC physfs-static)

find_package(Bullet)
target_link_libraries(World PUBLIC Bullet::Bullet)

find_package(SFML REQUIRED COMPONENTS sfml-window)
target_link_libraries(Framework PUBLIC sfml-window)
