
add_library(World STATIC
    include/Volcano/World/Config.h.in
    include/Volcano/World/Common.h
    include/Volcano/World/Component.h
    include/Volcano/World/Config.h.in
    include/Volcano/World/Entity.h
    include/Volcano/World/Scene.h
    include/Volcano/World/Transform.h
    include/Volcano/World/Transformable.h
    src/Volcano/World/Component.cpp
    src/Volcano/World/Entity.cpp
    src/Volcano/World/Scene.cpp
    src/Volcano/World/Transform.cpp
    src/Volcano/World/Transformable.cpp
)

configure_file(
    include/Volcano/World/Config.h.in
    include/Volcano/World/Config.h
)

set_target_properties(World PROPERTIES OUTPUT_NAME VolcanoWorld)

target_include_directories(World PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(World PUBLIC Base)

find_package(EnTT REQUIRED)
target_link_libraries(World PUBLIC EnTT::EnTT)

volcano_link_qt(World Quick)
volcano_setup_target(World)
