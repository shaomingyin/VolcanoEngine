
macro(volcano_target_post_setup TARGET)
    get_target_property(TARGET_LOCATION ${TARGET} SOURCE_DIR)
    if(NOT ${TARGET_LOCATION} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
        message(FATAL_ERROR "Macro volcano_target_post_setup('${TARGET}') must be used within the directory where the target defined.")
    endif()
    string(TOLOWER ${TARGET} TARGET_LOWER)
    set_target_properties(${TARGET} PROPERTIES
        OUTPUT_NAME volcano-${TARGET_LOWER}
        INTERPROCEDURAL_OPTIMIZATION ON
        )
    get_target_property(TARGET_SOURCES ${TARGET} SOURCES)
    source_group(TREE ${TARGET_LOCATION} FILES ${TARGET_SOURCES})
endmacro()

