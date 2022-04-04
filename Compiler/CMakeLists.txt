
link_libraries(Runtime)
link_qt_modules(Core)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

glob_sources(COMPILER_SOURCES)
qt_add_executable(Compiler ${COMPILER_SOURCES})
set_target_properties(Compiler PROPERTIES OUTPUT_NAME "VolcanoCompiler")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${COMPILER_SOURCES})
