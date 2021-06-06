//
//
#include <thread>

#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(const Napi::CallbackInfo &info):
	Node::Object<World>(info)
{
}

World::~World(void)
{
}

void World::registerClass(Napi::Env env)
{
	registerConstructor(env, "World", {});
}

void World::update(Duration elapsed)
{
	// TODO
}

VOLCANO_GAME_END
