
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
link_libraries(Qt6::Core)

add_subdirectory(base base)
add_subdirectory(editor editor)
add_subdirectory(launcher launcher)
add_subdirectory(server server)
