
macro(volcano_target_setup TARGET)
	get_target_property(TARGET_SOURCES ${TARGET} SOURCES)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_SOURCES})
	file(RELATIVE_PATH RDIR ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    if(RDIR)
        set_target_properties(${TARGET} PROPERTIES FOLDER ${RDIR})
    endif()
endmacro()

