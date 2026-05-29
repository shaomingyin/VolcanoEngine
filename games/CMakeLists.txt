
set(VOLCANO_GAMES)

file(GLOB DENTS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach(DENT ${DENTS})
	if(IS_READABLE ${DENT}/CMakeLists.txt)
		get_filename_component(DIR ${DENT} NAME)
		list(APPEND VOLCANO_GAMES ${DIR})
	endif()
endforeach()

if(NOT VOLCANO_GAMES)
	message(FATAL_ERROR "No game found.")
endif()

list(GET VOLCANO_GAMES 0 VOLCANO_GAME_DEFAULT)
set(VOLCANO_GAME ${VOLCANO_GAME_DEFAULT} CACHE STRING "Specify the game.")
set_property(CACHE VOLCANO_GAME PROPERTY STRINGS ${VOLCANO_GAMES})
add_subdirectory(${VOLCANO_GAME})
