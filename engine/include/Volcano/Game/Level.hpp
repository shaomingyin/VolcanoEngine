//
//
#ifndef VOLCANO_GAME_LEVEL_HPP
#define VOLCANO_GAME_LEVEL_HPP

#include <sigslot/signal.hpp>
#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Node.hpp>
#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Level: public Node::Object<Level> {

public:
	Level(const Napi::CallbackInfo &info);
	virtual ~Level(void);

public:
	static Napi::Function defineConstructor(Napi::Env env);
	void update(Duration elapsed);

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

#endif // VOLCANO_GAME_LEVEL_HPP
