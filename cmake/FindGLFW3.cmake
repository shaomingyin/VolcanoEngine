#
#
#

find_path(GLFW3_INCLUDE_DIR GLFW/glfw3.h
  HINTS
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

find_library(GLFW3_LIBRARY 
  NAMES glfw3
  HINTS
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(glfw3 DEFAULT_MSG GLFW3_INCLUDE_DIR GLFW3_LIBRARY)

mark_as_advanced(GLFW3_INCLUDE_DIR GLFW3_LIBRARY)

if(GLFW3_FOUND)
	set(GLFW3_INCLUDE_DIRS ${GLFW3_INCLUDE_DIR})
	set(GLFW3_LIBRARIES ${GLFW3_LIBRARY})
endif()
