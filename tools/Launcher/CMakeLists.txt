
add_executable(VolcanoLauncher
	Main.cpp
	)

add_dependencies(VolcanoLauncher
    Volcano
	)

target_link_libraries(VolcanoLauncher
    Volcano
    Qt5::QuickControls2
    Qt5::Quick
    Qt5::Qml
    Qt5::Gui
	)
