
add_executable(VolcanoLauncher
	Main.cpp
	MainWindow.cpp
	MainWindow.hpp
	)

add_dependencies(VolcanoLauncher Volcano)
target_link_libraries(VolcanoLauncher Volcano)

if(UNIX)
    target_link_libraries(VolcanoLauncher pthread)
endif()
