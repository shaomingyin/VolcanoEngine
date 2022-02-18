
link_qt_modules(Core)

glob_sources(RUNTIME_SOURCES)
qt_add_library(Runtime STATIC ${RUNTIME_SOURCES})
set_target_properties(Runtime PROPERTIES OUTPUT_NAME "VolcanoRuntime")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RUNTIME_SOURCES})

target_include_directories(Runtime
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_compile_definitions(Runtime
	PUBLIC
		WIN32_LEAN_AND_MEAN
	)

target_link_qt_modules(Runtime
	PUBLIC
		Concurrent
		Qml
		Gui
		OpenGL
		Quick
    )

if(WIN32)
    target_link_libraries(Runtime PUBLIC Winmm)
endif()

include(FindBullet)
if(BULLET_FOUND)
    target_include_directories(Runtime PUBLIC ${BULLET_INCLUDE_DIRS})
    target_link_libraries(Runtime PUBLIC ${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Bullet not found.")
endif()

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )
