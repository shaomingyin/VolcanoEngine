
add_library(Simulation STATIC
    include/Volcano/Simulation/Common.hpp
    include/Volcano/Simulation/Config.hpp.in
    include/Volcano/Simulation/Context.hpp
    include/Volcano/Simulation/Scene.hpp
    include/Volcano/Simulation/World.hpp
    src/Volcano/Simulation/Common.cpp
    src/Volcano/Simulation/Context.cpp
    src/Volcano/Simulation/Scene.cpp
    src/Volcano/Simulation/World.cpp
)

configure_file(
    include/Volcano/Simulation/Config.hpp.in
    include/Volcano/Simulation/Config.hpp
)

target_include_directories(Simulation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Simulation)
set_target_properties(Simulation PROPERTIES OUTPUT_NAME VolcanoSimulation)

target_link_libraries(Simulation PUBLIC Base)

find_package(Async++ REQUIRED)
target_link_libraries(Simulation PUBLIC Async++)

find_package(PhysFS REQUIRED)
target_link_libraries(Simulation PUBLIC physfs-static)

find_package(EnTT REQUIRED)
target_link_libraries(Simulation PUBLIC EnTT::EnTT)

find_package(Edyn REQUIRED)
target_link_libraries(Simulation PUBLIC Edyn::Edyn)
