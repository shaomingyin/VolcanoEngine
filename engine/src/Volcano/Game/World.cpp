//
//
#include <thread>

#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

Napi::FunctionReference World::c_constructorRef;

World::World(const Napi::CallbackInfo &info):
	Napi::ObjectWrap<World>(info),
	m_gravity(0.0f, -9.8f, 0.0f),
	m_dynamicWorld(nullptr)
{
}

World::~World(void)
{
	releaseDynamic();
}

Napi::Function World::constructor(Napi::Env env)
{
	return Node::defineClass<World>(env, "World", {});
}

void World::update(Duration elapsed)
{
	// TODO
}

VOLCANO_GAME_END
