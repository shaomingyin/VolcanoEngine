
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Concurrent Qml REQUIRED)
link_libraries(Qt6::Core Qt6::Concurrent Qt6::Qml)

# runtimes
add_subdirectory(base base)
add_subdirectory(game game)
add_subdirectory(graphics graphics)
add_subdirectory(sound sound)

# executables
add_subdirectory(launcher launcher)
add_subdirectory(server server)
add_subdirectory(editor editor)
