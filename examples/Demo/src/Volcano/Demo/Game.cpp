//
//
#include <Volcano/Demo/Game.h>

VOLCANO_DEMO_BEGIN

Game::Game(Framework::Context& context)
	: Framework::Game(context) {
}

VOLCANO_DEMO_END

#include <rttr/registration>

RTTR_REGISTRATION {
	rttr::registration::class_<Volcano::Demo::Game>("VolcanoDemoGame")(
			rttr::metadata("Name", "Demo"),
			rttr::metadata("Description", "This is a demo game.")
		).constructor<Volcano::Framework::Context&>();
}
