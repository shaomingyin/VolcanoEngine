//
//
#include <rttr/registration>

#include <Volcano/Demo/Game.h>

VOLCANO_DEMO_BEGIN

Game::Game(Framework::Context& context)
	: Framework::Game(context) {
}

VOLCANO_DEMO_END

RTTR_PLUGIN_REGISTRATION {
	rttr::registration::class_<Volcano::Demo::Game>("VolcanoDemoGame")
		.constructor<Volcano::Framework::Context&>();
}
