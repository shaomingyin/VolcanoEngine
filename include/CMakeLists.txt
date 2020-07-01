
add_subdirectory(GL GL)
add_subdirectory(glm glm)
add_subdirectory(KHR KHR)
add_subdirectory(Volcano Volcano)

target_sources(Volcano
	PUBLIC
		GL.hpp
		nanovg.h
		Volcano.hpp
		pugiconfig.hpp
		pugixml.hpp
	PRIVATE
		fontstash.h
		nanovg_gl.h
		nanovg_gl_utils.h
		stb_image.h
		stb_truetype.h
	)
