
target_sources(Volcano
	PRIVATE
        Driver.cpp
        FileSystem.cpp
		Module.cpp
		Resource.cpp
		VM.cpp
        ZipArchive.cpp
        ZipFileSystem.cpp
	)

add_subdirectory(Drivers Drivers)
add_subdirectory(Gui Gui)
add_subdirectory(Scene Scene)
