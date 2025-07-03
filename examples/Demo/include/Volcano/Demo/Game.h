//
//
#ifndef VOLCANO_DEMO_GAME_H
#define VOLCANO_DEMO_GAME_H

#include <Volcano/Framework/Game.h>
#include <Volcano/Demo/Common.h>

VOLCANO_DEMO_BEGIN

class Game: public Framework::Game {
public:
	Game(Framework::Context& context);
};

VOLCANO_DEMO_END

#endif // VOLCANO_DEMO_GAME_H
