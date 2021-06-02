//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Game/SinglePlayer.hpp>

VOLCANO_GAME_BEGIN

Napi::FunctionReference SinglePlayer::c_constructorRef;

SinglePlayer::SinglePlayer(const Napi::CallbackInfo &info) :
	Napi::ObjectWrap<SinglePlayer>(info)
{
	auto worldObject = World::newInstance({});
	m_world = World::Unwrap(worldObject);
	if (m_world == nullptr) {
		Napi::ThrowError(info.Env(), "Faield to create world object.");
		return;
	}

	m_worldRef = Napi::Persistent(worldObject);
}

SinglePlayer::~SinglePlayer(void)
{
}

Napi::Function SinglePlayer::constructor(Napi::Env env)
{
	return Node::defineClass<SinglePlayer, Frame>(env, "SinglePlayer", {
		InstanceMethod<&SinglePlayer::start>("start"),
		InstanceMethod<&SinglePlayer::stop>("stop"),
		InstanceMethod<&SinglePlayer::isStarted>("isStarted")
	});
}

void SinglePlayer::update(Duration elapsed)
{
	if (m_world != nullptr)
		m_world->update(elapsed);
}

Napi::Value SinglePlayer::start(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), true);
}

Napi::Value SinglePlayer::stop(const Napi::CallbackInfo &info)
{
	return Napi::Value();
}

Napi::Value SinglePlayer::world(const Napi::CallbackInfo &info)
{
	return m_worldRef.Value();
}

VOLCANO_GAME_END
