#
#
#

find_path(UV_INCLUDE_DIR uv.h
  HINTS
  $ENV{UV_DIR}
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

find_library(UV_LIBRARY 
  NAMES uv
  HINTS
  $ENV{UV_DIR}
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
find_package_handle_standard_args(uv DEFAULT_MSG UV_INCLUDE_DIR UV_LIBRARY)

mark_as_advanced(UV_INCLUDE_DIR UV_LIBRARY)

if(UV_FOUND)
	set(UV_INCLUDE_DIRS ${UV_INCLUDE_DIR})
	set(UV_LIBRARIES ${UV_LIBRARY})
endif()

