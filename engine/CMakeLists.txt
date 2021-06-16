
find_package(Qt6 COMPONENTS Core REQUIRED)
link_libraries(Qt6::Core)

add_subdirectory(runtime runtime)
add_subdirectory(launcher launcher)
add_subdirectory(editor editor)
