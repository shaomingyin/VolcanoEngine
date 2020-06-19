
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Concurrent REQUIRED)

add_library(VulkanRenderer SHARED)

add_dependencies(VulkanRenderer
	Base
	)

target_compile_definitions(VulkanRenderer
	PRIVATE
		VOLCANO_VULKANRENDERER
	)

target_link_libraries(VulkanRenderer
	Base
	Qt5::Core
	Qt5::Gui
	Qt5::Concurrent
	)

set_target_properties(VulkanRenderer
	PROPERTIES
		OUTPUT_NAME "VolcanoVulkanRenderer"
	)

add_subdirectory(include include)
add_subdirectory(src src)
