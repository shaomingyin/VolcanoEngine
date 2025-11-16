
add_library(Framework STATIC
    include/Volcano/Framework/Base.hpp
    include/Volcano/Framework/Client.hpp
    include/Volcano/Framework/Common.hpp
    include/Volcano/Framework/Config.hpp.in
    include/Volcano/Framework/Console.hpp
    include/Volcano/Framework/Context.hpp
    include/Volcano/Framework/Local.hpp
    include/Volcano/Framework/Server.hpp
    include/Volcano/Framework/Window.hpp
    include/Volcano/Framework/World.hpp
    src/Volcano/Framework/Base.cpp
    src/Volcano/Framework/Client.cpp
    src/Volcano/Framework/Common.cpp
    src/Volcano/Framework/Console.cpp
    src/Volcano/Framework/Context.cpp
    src/Volcano/Framework/Local.cpp
    src/Volcano/Framework/Server.cpp
    src/Volcano/Framework/Window.cpp
    src/Volcano/Framework/World.cpp
)

configure_file(
    include/Volcano/Framework/Config.hpp.in
    include/Volcano/Framework/Config.hpp
)

target_include_directories(Framework PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Framework)
set_target_properties(Framework PROPERTIES OUTPUT_NAME VolcanoFramework)

target_link_libraries(Framework PUBLIC Scene Graphics Gui Vm)

find_package(PhysFS)
target_link_libraries(Framework PUBLIC physfs-static)

find_package(Edyn REQUIRED)
target_link_libraries(Framework PUBLIC Edyn::Edyn)

find_package(SFML REQUIRED COMPONENTS sfml-window)
target_link_libraries(Framework PUBLIC sfml-window)
