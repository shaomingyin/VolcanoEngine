
function(volcano_enable_if COND)
	if(COND)
		return(${ARGN})
	else()
		return()
	endif()
endfunction()

function(volcano_option OPT DESC COND)
	if(${COND})
		set(VAL ON)
	else()
		set(VAL OFF)
	endif()
	message(STATUS "Option: ${OPT}\t= ${VAL}")
	set(${OPT} ${VAL} CACHE BOOL ${DESC})
endfunction()

macro(volcano_set_target_folder TARGET)
    file(RELATIVE_PATH RDIR ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    if(RDIR)
        set_target_properties(${TARGET} PROPERTIES FOLDER ${RDIR})
    endif()
endmacro()

macro(volcano_source_tree)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ARGN})
endmacro()

function(volcano_setup_target TARGET)
    set(FLAG_ARGS "")
    set(ONE_VALUE_ARGS
		FOLDER
        OUTPUT_NAME
    )
    set(MULTI_VALUE_ARGS
		DEPENDENCIES
		PUBLIC_HEADERS
        PUBLIC_SOURCES
        PRIVATE_SOURCES
        PUBLIC_INCLUDE_DIRECTORIES
        PRIVATE_INCLUDE_DIRECTORIES
        PUBLIC_DEFINITIONS
        PRIVATE_DEFINITIONS
        PUBLIC_LINK_DIRECTORIES
        PRIVATE_LINK_DIRECTORIES
        PUBLIC_LINK_LIBRARIES
        PRIVATE_LINK_LIBRARIES
		PUBLIC_LINK_PACKAGES
		PRIVATE_LINK_PACKAGES
		PUBLIC_QT_MODULES
		PRIVATE_QT_MODULES
        EXTRA_INSTALL_FILES
		EXTRA_INSTALL_DIRECTORIES
    )
	
    cmake_parse_arguments(ARG "${FLAG_ARGS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN})
	
	if(ARG_FOLDER)
		message(STATUS "  Folder: ${ARG_FOLDER}")
		set_target_properties(${TARGET} PROPERTIES FOLDER ${ARG_FOLDER})
	else()
		file(RELATIVE_PATH RDIR ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
		if(RDIR)
			message(STATUS "  Folder: ${RDIR} (auto)")
			set_target_properties(${TARGET} PROPERTIES FOLDER ${RDIR})
		endif()
	endif()
	
    if(ARG_OUTPUT_NAME)
		message(STATUS "  Output name: ${ARG_OUTPUT_NAME}")
        set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${ARG_OUTPUT_NAME})
		string(MAKE_C_IDENTIFIER ${ARG_OUTPUT_NAME} TARGET_ID)
	else()
		string(MAKE_C_IDENTIFIER ${TARGET} TARGET_ID)
	endif()

	set(TARGET_EXPORTS "${TARGET_ID}_EXPORTS")
	message(STATUS "  Export: ${TARGET_EXPORTS}")
	target_compile_definitions(${TARGET} PRIVATE ${TARGET_EXPORTS})

	get_target_property(TARGET_TYPE ${TARGET} TYPE)
	if(TARGET_TYPE STREQUAL "SHARED_LIBRARY")
		set(${TARGET}_SHARED ON)
	endif()

	if(ARG_PUBLIC_HEADERS)
		list(POP_BACK ARG_PUBLIC_HEADERS PUBLIC_HEADERS_DESTINATION DESTINATION_KEY)
		if(NOT ARG_PUBLIC_HEADERS)
			message(FATAL_ERROR "Empty PUBLIC_HEADERS.")
		endif()
		if(PUBLIC_HEADERS_DESTINATION AND DESTINATION_KEY)
			if(NOT DESTINATION_KEY STREQUAL "DESTINATION")
				message(FATAL_ERROR "Set PUBLIC_HEADERS without DESTINATION.")
			endif()
			set_target_properties(${TARGET} PROPERTIES PUBLIC_HEADER "${ARG_PUBLIC_HEADERS}")
		else()
			message(FATAL_ERROR "Invalid PUBLIC_HEADERS.")
		endif()
	else()
		set(PUBLIC_HEADERS_DESTINATION "include")
	endif()
	
	if(ARG_DEPENDENCIES)
		message(STATUS "  Denendencies:")
		foreach(DEP ${ARG_DEPENDENCIES})
			message(STATUS "    ${DEP}")
			add_dependencies(${TARGET} ${DEP})
		endforeach()
	endif()

    if(ARG_PUBLIC_SOURCES)
        target_sources(${TARGET} PUBLIC ${ARG_PUBLIC_SOURCES})
    endif()

    if(ARG_PRIVATE_SOURCES)
        target_sources(${TARGET} PRIVATE ${ARG_PRIVATE_SOURCES})
    endif()

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ARG_PUBLIC_SOURCES} ${ARG_PRIVATE_SOURCES})

    foreach(TARGET_SOURCE ${ARG_PUBLIC_SOURCES} ${ARG_PRIVATE_SOURCES})
        if(TARGET_SOURCE MATCHES "\\.in$")
            file(REAL_PATH ${TARGET_SOURCE} TARGET_SOURCE_TEMPLATE)
            file(RELATIVE_PATH TARGET_SOURCE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR} ${TARGET_SOURCE_TEMPLATE})
            string(REGEX REPLACE "\\.in$" "" TARGET_SOURCE_TEMPLATE_OUTPUT ${TARGET_SOURCE_TEMPLATE})
			message(STATUS "  Configuring ${TARGET_SOURCE_TEMPLATE} to ${TARGET_SOURCE_TEMPLATE_OUTPUT}...")
            configure_file(${TARGET_SOURCE_TEMPLATE} ${TARGET_SOURCE_TEMPLATE_OUTPUT})
        endif()
    endforeach()
	
	if(ARG_PUBLIC_INCLUDE_DIRECTORIES)
		message(STATUS "  Public include directories:")
		foreach(PUBLIC_INCLUDE_DIRECTORY ${ARG_PUBLIC_INCLUDE_DIRECTORIES})
			message(STATUS "    ${PUBLIC_INCLUDE_DIRECTORY}")
			target_include_directories(${TARGET} PUBLIC ${PUBLIC_INCLUDE_DIRECTORY})
		endforeach()
	endif()
	
	if(ARG_PRIVATE_INCLUDE_DIRECTORIES)
		message(STATUS "  Private include directories:")
		foreach(PRIVATE_INCLUDE_DIRECTORY ${ARG_PRIVATE_INCLUDE_DIRECTORIES})
			message(STATUS "    ${PRIVATE_INCLUDE_DIRECTORY}")
			target_include_directories(${TARGET} PRIVATE ${PRIVATE_INCLUDE_DIRECTORY})
		endforeach()
	endif()

	if(ARG_PUBLIC_DEFINITIONS)
		message(STATUS "  Public definitions:")
		foreach(PUBLIC_DEFINITION ${ARG_PUBLIC_DEFINITIONS})
			message(STATUS "    ${PUBLIC_DEFINITION}")
			target_compile_definitions(${TARGET} PUBLIC ${PUBLIC_DEFINITION})
		endforeach()
	endif()

	if(ARG_PRIVATE_DEFINITIONS)
		message(STATUS "  Private definitions:")
		foreach(PRIVATE_DEFINITION ${ARG_PRIVATE_DEFINITIONS})
			message(STATUS "    ${PRIVATE_DEFINITION}")
			target_compile_definitions(${TARGET} PRIVATE ${PRIVATE_DEFINITION})
		endforeach()
	endif()

	if(ARG_PUBLIC_LINK_DIRECTORIES)
		message(STATUS "  Public link directories:")
		foreach(PUBLIC_LINK_DIRECTORY ${ARG_PUBLIC_LINK_DIRECTORIES})
			message(STATUS "    ${PUBLIC_LINK_DIRECTORY}")
			target_link_directories(${TARGET} PUBLIC ${PUBLIC_LINK_DIRECTORY})
		endforeach()
	endif()
	
	if(ARG_PRIVATE_LINK_DIRECTORIES)
		message(STATUS "  Private link directories:")
		foreach(PRIVATE_LINK_DIRECTORY ${ARG_PRIVATE_LINK_DIRECTORIES})
			message(STATUS "    ${PRIVATE_LINK_DIRECTORY}")
			target_link_directories(${TARGET} PRIVATE ${PRIVATE_LINK_DIRECTORY})
		endforeach()
	endif()
	
	if(ARG_PUBLIC_LINK_LIBRARIES)
		message(STATUS "  Public link libraries:")
		foreach(PUBLIC_LINK_LIBRARY ${ARG_PUBLIC_LINK_LIBRARIES})
			message(STATUS "    ${PUBLIC_LINK_LIBRARY}")
			target_link_libraries(${TARGET} PUBLIC ${PUBLIC_LINK_LIBRARY})
		endforeach()
	endif()
	
	if(ARG_PRIVATE_LINK_LIBRARIES)
		message(STATUS "  Private link libraries:")
		foreach(PRIVATE_LINK_LIBRARY ${ARG_PRIVATE_LINK_LIBRARIES})
			message(STATUS "    ${PRIVATE_LINK_LIBRARY}")
			target_link_libraries(${TARGET} PRIVATE ${PRIVATE_LINK_LIBRARY})
		endforeach()
	endif()
	
	if(ARG_PUBLIC_LINK_PACKAGES)
		message(STATUS "  Public link packages:")
		while(ARG_PUBLIC_LINK_PACKAGES)
			list(POP_FRONT ARG_PUBLIC_LINK_PACKAGES PACKAGE_FN PACKAGE_LN)
			if(PACKAGE_FN AND PACKAGE_LN)
				message(STATUS "    ${PACKAGE_FN} - ${PACKAGE_LN}")
				find_package(${PACKAGE_FN} REQUIRED)
				target_link_libraries(${TARGET} PUBLIC ${PACKAGE_LN})
			else()
				message(FATAL_ERROR "Invalid PUBLIC_LINK_PACKAGES.")
			endif()
		endwhile()
	endif()
	
	if(ARG_PRIVATE_LINK_PACKAGES)
		message(STATUS "  Private link packages:")
		while(ARG_PRIVATE_LINK_PACKAGES)
			list(POP_FRONT ARG_PRIVATE_LINK_PACKAGES PACKAGE_FN PACKAGE_LN)
			if(PACKAGE_FN AND PACKAGE_LN)
				message(STATUS "    ${PACKAGE_FN} - ${PACKAGE_LN}")
				find_package(${PACKAGE_FN} REQUIRED)
				target_link_libraries(${TARGET} RPIVATE ${PACKAGE_LN})
			else()
				message(FATAL_ERROR "Invalid PRIVATE_LINK_PACKAGES.")
			endif()
		endwhile()
	endif()
	
	if(ARG_PUBLIC_QT_MODULES)
		message(STATUS "  Public link Qt modules:")
		foreach(PUBLIC_QT_MODULE ${ARG_PUBLIC_QT_MODULES})
			message(STATUS "    ${PUBLIC_QT_MODULE}")
			find_package(Qt6 REQUIRED COMPONENTS ${PUBLIC_QT_MODULE})
			target_link_libraries(${TARGET} PUBLIC Qt6::${PUBLIC_QT_MODULE})
		endforeach()
	endif()
	
	if(ARG_PRIVATE_QT_MODULES)
		message(STATUS "  Private link Qt modules:")
		foreach(PRIVATE_QT_MODULE ${ARG_PRIVATE_QT_MODULES})
			message(STATUS "    ${PRIVATE_QT_MODULE}")
			find_package(Qt6 REQUIRED COMPONENTS ${PRIVATE_QT_MODULE})
			target_link_libraries(${TARGET} PRIVATE Qt6::${PRIVATE_QT_MODULE})
		endforeach()
	endif()
	
	install(TARGETS ${TARGET}
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION bin
		PUBLIC_HEADER DESTINATION ${PUBLIC_HEADERS_DESTINATION}
	)
	
	if(ARG_EXTRA_INSTALL_FILES)
		list(POP_BACK ARG_EXTRA_INSTALL_FILES INSTALL_FILES_DESTINATION DESTINATION_KEY)
		if(INSTALL_FILES_DESTINATION AND DESTINATION_KEY)
			if(NOT ARG_EXTRA_INSTALL_FILES)
				message(FATAL_ERROR "Empty INSTALL_FILES.")
			endif()
			if(NOT DESTINATION_KEY STREQUAL "DESTINATION")
				message(FATAL_ERROR "Set INSTALL_FILES without DESTINATION.")
			endif()
			install(FILES ${ARG_EXTRA_INSTALL_FILES} DESTINATION ${INSTALL_FILES_DESTINATION})
		else()
			message(FATAL_ERROR "Invalid INSTALL_FILES.")
		endif()
	endif()
	
	if(ARG_EXTRA_INSTALL_DIRECTORIES)
		list(POP_BACK ARG_EXTRA_INSTALL_DIRECTORIES INSTALL_DIRECTORIES_DESTINATION DESTINATION_KEY)
		if(INSTALL_DIRECTORIES_DESTINATION AND DESTINATION_KEY)
			if(NOT ARG_EXTRA_INSTALL_DIRECTORIES)
				message(FATAL_ERROR "Empty INSTALL_DIRECTORIES.")
			endif()
			if(NOT DESTINATION_KEY STREQUAL "DESTINATION")
				message(FATAL_ERROR "Set INSTALL_DIRECTORIES without DESTINATION.")
			endif()
			install(FILES ${ARG_EXTRA_INSTALL_DIRECTORIES} DESTINATION ${INSTALL_DIRECTORIES_DESTINATION})
		else()
			message(FATAL_ERROR "Invalid INSTALL_DIRECTORIES.")
		endif()
	endif()
endfunction()

function(volcano_add_static_library TARGET)
	message(STATUS "Add static library: ${CMAKE_CURRENT_SOURCE_DIR} - ${TARGET}")
    qt_add_library(${TARGET} STATIC)
    volcano_setup_target(${TARGET} ${ARGN})
endfunction()

function(volcano_add_shared_library TARGET)
	message(STATUS "Add shared library: ${CMAKE_CURRENT_SOURCE_DIR} - ${TARGET}")
    qt_add_library(${TARGET} SHARED)
    volcano_setup_target(${TARGET} ${ARGN})
    if(MSVC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
endfunction()

function(volcano_add_executable TARGET)
	message(STATUS "Add executable: ${CMAKE_CURRENT_SOURCE_DIR} - ${TARGET}")
	qt_add_executable(${TARGET})
    volcano_setup_target(${TARGET} ${ARGN})
endfunction()

function(volcano_add_auto_executable TARGET)
	message(STATUS "Add executable: ${CMAKE_CURRENT_SOURCE_DIR} - ${TARGET}")
	qt_add_executable(${TARGET})
    volcano_setup_target(${TARGET} ${ARGN})
endfunction()
