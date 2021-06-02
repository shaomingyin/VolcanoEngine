//
//
#include <Volcano/Node.hpp>
#include <Volcano/Game/Level.hpp>

VOLCANO_GAME_BEGIN

Level::Level(const Napi::CallbackInfo &info) :
	Napi::ObjectWrap<Level>(info),
	m_gravity(0.0f, -9.8f, 0.0f),
	m_dynamicWorld(nullptr)
{
}

Level::~Level(void)
{
	releaseDynamic();
}

Napi::Function Level::constructor(Napi::Env env)
{
	return Node::defineClass<Level>(env, "World", {
		InstanceAccessor<&Level::isDynamic, &Level::setDynamic>("dynamic"),
		InstanceAccessor<&Level::gravity, &Level::setGravity>("gravity")
	});
}

void Level::update(Duration elapsed)
{
	// TODO
}

Napi::Value Level::isDynamic(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_dynamicWorld != nullptr);
}

void Level::setDynamic(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (value.ToBoolean())
		createDynamic();
	else
		releaseDynamic();
}

Napi::Value Level::gravity(const Napi::CallbackInfo &info)
{
	auto value = Napi::Array::New(info.Env());

	value[0u] = m_gravity.x();
	value[1u] = m_gravity.y();
	value[2u] = m_gravity.z();

	return value;
}

void Level::setGravity(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	auto obj = Napi::Object(value.Env(), value);

	m_gravity.x() = obj.Get(0u).ToNumber();
	m_gravity.y() = obj.Get(1u).ToNumber();
	m_gravity.z() = obj.Get(2u).ToNumber();

	if (m_dynamicWorld != nullptr)
		m_dynamicWorld->setGravity(btVector3(m_gravity[0], m_gravity[1], m_gravity[2]));
}

bool Level::createDynamic(void)
{
	if (m_dynamicWorld != nullptr)
		return true;

	// TODO

	return false;
}

void Level::releaseDynamic(void)
{
	if (m_dynamicWorld == nullptr)
		return;

	// TODO
}

VOLCANO_GAME_END
