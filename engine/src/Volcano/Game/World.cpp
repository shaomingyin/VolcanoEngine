//
//
#include <thread>

#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

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

Napi::Function World::defineClass(Napi::Env env)
{
	return DefineClass(env, "World", {
		InstanceAccessor<&World::isDynamic, &World::setDynamic>("dynamic"),
		InstanceAccessor<&World::gravity, &World::setGravity>("gravity")
	});
}

void World::update(float elapsed)
{
	// handleClients(elapsed);

	// update ecs world.

	if (m_dynamicWorld != nullptr)
		m_dynamicWorld->stepSimulation(elapsed);

	// dispatchClients(elapsed);
}

Napi::Value World::isDynamic(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_dynamicWorld != nullptr);
}

void World::setDynamic(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (value.ToBoolean())
		createDynamic();
	else
		releaseDynamic();
}

Napi::Value World::gravity(const Napi::CallbackInfo &info)
{
	auto value = Napi::Array::New(info.Env());

	value[0u] = m_gravity.x();
	value[1u] = m_gravity.y();
	value[2u] = m_gravity.z();

	return value;
}

void World::setGravity(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	auto obj = Napi::Object(value.Env(), value);

	m_gravity.x() = obj.Get(0u).ToNumber();
	m_gravity.y() = obj.Get(1u).ToNumber();
	m_gravity.z() = obj.Get(2u).ToNumber();

	if (m_dynamicWorld != nullptr)
		m_dynamicWorld->setGravity(btVector3(m_gravity[0], m_gravity[1], m_gravity[2]));
}

bool World::createDynamic(void)
{
	if (m_dynamicWorld != nullptr)
		return true;

	// TODO

	return false;
}

void World::releaseDynamic(void)
{
	if (m_dynamicWorld == nullptr)
		return;

	// TODO
}

VOLCANO_GAME_END
