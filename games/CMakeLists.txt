#
#
#

set(GAME_DIRS)
set(GAME_DIR "" CACHE STRING "Current game.")

file(GLOB DENTS LIST_DIRECTORIES true RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")
foreach(DENT ${DENTS})
	message(">>>>>>>>>>>> ${DENT}")
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DENT}/CMakeLists.txt")
		list(APPEND GAME_DIRS ${DENT})
	else()
		message(">>>>>>>> ${DENT}/CMakeLists.txt not existed.")
	endif()
endforeach()

set_property(CACHE GAME_DIR PROPERTY STRINGS ${GAME_DIRS})
if(EXISTS ${GAME_DIR})
	add_subdirectory(${GAME_DIR})
endif()
