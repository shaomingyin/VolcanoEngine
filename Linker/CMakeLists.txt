
link_libraries(Runtime)
link_qt_modules(Core)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

glob_sources(LINKER_SOURCES)
qt_add_executable(Linker ${LINKER_SOURCES})
set_target_properties(Linker PROPERTIES OUTPUT_NAME "VolcanoLinker")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LINKER_SOURCES})
