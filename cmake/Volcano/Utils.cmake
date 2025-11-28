
macro(volcano_source_group TARGET)
	get_target_property(TARGET_SOURCES ${TARGET} SOURCES)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_SOURCES})
endmacro()

macro(volcano_setup_target TARGET)
    file(RELATIVE_PATH RDIR ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    if(RDIR)
        set_target_properties(${TARGET} PROPERTIES FOLDER ${RDIR})
	endif()
	volcano_source_group(${TARGET})
endmacro()

macro(volcano_link_qt TARGET MODULE)
	find_package(Qt6 REQUIRED COMPONENTS ${MODULE})
	target_link_libraries(${TARGET} PUBLIC Qt6::${MODULE})
endmacro()

macro(volcano_link_package TARGET PACKAGE_NAME LINK_NAME)
	find_package(${PACKAGE_NAME} REQUIRED)
	target_link_libraries(${TARGET} PUBLIC ${LINK_NAME})
endmacro()
