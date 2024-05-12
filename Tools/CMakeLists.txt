
set(VOLCANO_TOOLS ON CACHE BOOL "Enable tools.")
if(NOT VOLCANO_TOOLS)
    return()
endif()

add_subdirectory(Server)
add_subdirectory(Launcher)
add_subdirectory(Editor)
