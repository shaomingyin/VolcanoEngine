
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(AUTOMOC ON)
set(AUTORCC ON)
set(AUTOUIC ON)

find_package(Qt6 REQUIRED Core)
link_libraries(Qt6::Core)

add_subdirectory(core core)
add_subdirectory(graphics graphics)
add_subdirectory(editor editor)
add_subdirectory(launcher launcher)
add_subdirectory(server server)
