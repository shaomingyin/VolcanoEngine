
add_library(World STATIC
    include/Volcano/World/Camera.h
    include/Volcano/World/Common.h
    include/Volcano/World/Config.h.in
    include/Volcano/World/Geometry.h
    include/Volcano/World/Inherent.h
    include/Volcano/World/Lighting.h
    include/Volcano/World/Map.h
    include/Volcano/World/Model.h
	include/Volcano/World/Physics.h
    include/Volcano/World/Scene.h
    include/Volcano/World/Screen.h
    include/Volcano/World/Session.h
    include/Volcano/World/Sound.h
    src/Volcano/World/Common.cpp
    src/Volcano/World/Physics.cpp
    src/Volcano/World/Scene.cpp
    src/Volcano/World/Session.cpp
)

configure_file(
    include/Volcano/World/Config.h.in
    include/Volcano/World/Config.h
)

target_include_directories(World PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(World)
set_target_properties(World PROPERTIES OUTPUT_NAME VolcanoWorld)

target_link_libraries(World PUBLIC Base)

find_package(EnTT)
target_link_libraries(World PUBLIC EnTT::EnTT)

find_package(Jolt)
target_link_libraries(World PUBLIC Jolt::Jolt)
