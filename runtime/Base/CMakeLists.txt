
add_library(Base STATIC
    include/Volcano/Common.h
    include/Volcano/Config.h.in
    include/Volcano/ScopeGuard.h
    include/Volcano/Version.h
    src/Volcano/Version.cpp
)

configure_file(
    include/Volcano/Config.h.in
    include/Volcano/Config.h
)

target_include_directories(Base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Base)
set_target_properties(Base PROPERTIES OUTPUT_NAME VolcanoBase)

find_package(Eigen3 REQUIRED)
target_link_libraries(Base Eigen3::Eigen)

find_package(Async++)
target_link_libraries(Base Async++)

find_package(SFML REQUIRED COMPONENTS sfml-system)
target_link_libraries(Base sfml-system)

find_package(PhysFS)
target_link_libraries(Base physfs-static)
