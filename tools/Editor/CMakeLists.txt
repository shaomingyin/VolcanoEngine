
find_package(Qt5Widgets REQUIRED)
link_libraries(Qt5::Widgets)

add_executable(VolcanoEditor
	Main.cpp
	MainWindow.cpp
	MainWindow.hpp
	)

add_dependencies(VolcanoEditor Volcano)
