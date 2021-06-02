//
//
#ifndef VOLCANO_GAME_SINGLEPLAYER_HPP
#define VOLCANO_GAME_SINGLEPLAYER_HPP

#include <Volcano/Node.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Frame.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

class SinglePlayer: public Frame {
public:
	SinglePlayer(const Napi::CallbackInfo &info);
	~SinglePlayer(void) override;

public:
	static Napi::Function constructor(Napi::Env env);

protected:
	bool init(void) override;
	void shutdown(void) override;
	void update(Duration elapsed) override;

private:
	Napi::Value world(const Napi::CallbackInfo &info);

private:
	Napi::ObjectReference m_worldRef;
	World *m_world;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SINGLEPLAYER_HPP
