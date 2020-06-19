
target_sources(BulletPhysics
	PUBLIC
		Common.h
		World.h
	PRIVATE
		Config.h.in
	)

configure_file(
	Config.h.in
	Config.h
	)
