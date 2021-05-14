
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

set(CMAKE_FOLDER engine)

add_subdirectory(base base)
add_subdirectory(graphics graphics)
add_subdirectory(input input)
add_subdirectory(sound sound)
add_subdirectory(gui gui)
add_subdirectory(rpc rpc)
add_subdirectory(game game)

add_subdirectory(server server)
add_subdirectory(launcher launcher)
add_subdirectory(editor editor)
