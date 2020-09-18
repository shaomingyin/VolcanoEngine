
target_sources(Volcano
	PUBLIC
		Common.hpp
		OpenGLBuffer.hpp
		OpenGLHeap.hpp
		OpenGLMemory.hpp
		OpenGLMesh.hpp
		OpenGLRenderer.hpp
    PRIVATE
        Config.hpp.in
	)

configure_file(
    Config.hpp.in
    Config.hpp
    )
