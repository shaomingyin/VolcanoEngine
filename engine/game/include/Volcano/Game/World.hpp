//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <sigslot/signal.hpp>
#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class World: public Napi::ObjectWrap<World> {
	VOLCANO_DISABLE_COPY_AND_MOVE(World)

public:
	World(const Napi::CallbackInfo &info);
	virtual ~World(void);

public:
	static void initExports(Napi::Env env, Napi::Object exports);
	void update(float elapsed);

private:
	Napi::Value isDynamic(const Napi::CallbackInfo &info);
	void setDynamic(const Napi::CallbackInfo &info, const Napi::Value &value);
	Napi::Value gravity(const Napi::CallbackInfo &info);
	void setGravity(const Napi::CallbackInfo &info, const Napi::Value &value);

private:
	bool createDynamic(void);
	void releaseDynamic(void);

private:
	entt::registry m_ecs;
	Eigen::Vector3f m_gravity;
	btDynamicsWorld *m_dynamicWorld;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
