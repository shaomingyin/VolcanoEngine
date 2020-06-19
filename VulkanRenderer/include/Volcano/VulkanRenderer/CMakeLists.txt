
target_sources(VulkanRenderer
	PUBLIC
		Common.h
		Context.h
	PRIVATE
		Config.h.in
	)

configure_file(
	Config.h.in
	Config.h
	)
