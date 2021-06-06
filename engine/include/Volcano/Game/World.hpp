//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <Volcano/Node.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Level.hpp>

VOLCANO_GAME_BEGIN

class World: public Node::Object<World> {

public:
	World(const Napi::CallbackInfo &info);
	virtual ~World(void);

public:
	static void registerClass(Napi::Env env);
	void update(Duration elapsed);

private:
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
