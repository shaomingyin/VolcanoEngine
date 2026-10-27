
add_subdirectory(Launcher)
add_subdirectory(Server)

option(VOLCANO_EDITOR "Build the Qt Creator editor plugin (requires its SDK)." OFF)
if(VOLCANO_EDITOR)
    add_subdirectory(Editor)
endif()
