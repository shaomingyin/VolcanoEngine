
set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug functions")
set(VOLCANO_EDITOR ON CACHE BOOL "Enable editor")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Concurrent REQUIRED)

add_library(Base SHARED)

target_compile_definitions(Base
	PRIVATE
		VOLCANO_BASE
	)

target_link_libraries(Base
	Qt5::Core
	Qt5::Gui
	Qt5::Concurrent
	)

set_target_properties(Base
	PROPERTIES
		OUTPUT_NAME "VolcanoBase"
	)

add_subdirectory(include include)
add_subdirectory(src src)
