
add_library(Base STATIC
    include/Volcano/Cache.hpp
    include/Volcano/Common.h
	include/Volcano/Common.hpp
    include/Volcano/Config.h.in
    include/Volcano/ScopeGuard.hpp
    include/Volcano/Version.hpp
    src/Volcano/Cache.cpp
    src/Volcano/Common.cpp
    src/Volcano/Error.cpp
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

find_package(spdlog REQUIRED)
target_link_libraries(Base PUBLIC spdlog::spdlog)

find_package(Eigen3 REQUIRED)
target_link_libraries(Base PUBLIC Eigen3::Eigen)
