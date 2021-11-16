
set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug funtions.")
set(VOLCANO_OUT_DIR "${CMAKE_BINARY_DIR}/out" CACHE PATH "Build output directory.")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${VOLCANO_OUT_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${VOLCANO_OUT_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${VOLCANO_OUT_DIR}/lib")

find_package(Qt6 COMPONENTS Core Concurrent Gui OpenGL Qml Quick REQUIRED)
link_libraries(Qt6::Core Qt6::Qml)

add_subdirectory(runtime runtime)
add_subdirectory(templates templates)
add_subdirectory(packages packages)
