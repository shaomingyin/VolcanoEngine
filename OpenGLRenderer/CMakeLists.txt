
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Concurrent REQUIRED)

add_library(OpenGLRenderer SHARED)

add_dependencies(OpenGLRenderer
	Base
	)

target_compile_definitions(OpenGLRenderer
	PRIVATE
		VOLCANO_OPENGLRENDERER
	)

target_link_libraries(OpenGLRenderer
	Base
	Qt5::Core
	Qt5::Gui
	Qt5::Concurrent
	)

set_target_properties(OpenGLRenderer
	PROPERTIES
		OUTPUT_NAME "VolcanoOpenGLRenderer"
	)

add_subdirectory(include include)
add_subdirectory(src src)
