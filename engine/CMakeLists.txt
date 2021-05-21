
if(WIN32)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

set(CMAKE_FOLDER engine)

add_subdirectory(base base)
add_subdirectory(graphics graphics)
add_subdirectory(sound sound)
add_subdirectory(gui gui)
add_subdirectory(game game)
add_subdirectory(client client)
