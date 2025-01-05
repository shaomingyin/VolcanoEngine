
volcano_add_executable(ModelCompiler
    OUTPUT_NAME
        VolcanoModelCompiler
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/ModelCompiler/Common.h
		include/Volcano/ModelCompiler/IOStream.h
		include/Volcano/ModelCompiler/IOSystem.h
		src/Volcano/ModelCompiler/IOStream.cpp
		src/Volcano/ModelCompiler/IOSystem.cpp
        src/Volcano/ModelCompiler/Main.cpp
    PUBLIC_LINK_LIBRARIES
        Resource
    PUBLIC_LINK_PACKAGES
        assimp assimp::assimp
)
