
target_sources(OpenGLRenderer
	PUBLIC
		Common.h
		Context.h
		Memory.h
		Mesh.h
	PRIVATE
		Config.h.in
	)

configure_file(
	Config.h.in
	Config.h
	)
