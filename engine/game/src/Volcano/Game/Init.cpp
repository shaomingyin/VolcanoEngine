//
//
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Game/Server.hpp>

VOLCANO_GAME_BEGIN

static napi_value init(napi_env env, napi_value exports)
{
	World::initExports(env, Napi::Object(env, exports));
	Server::initExports(env, Napi::Object(env, exports));

	return exports;
}

VOLCANO_GAME_END

NAPI_MODULE(NODE_GYP_MODULE_NAME, Volcano::Game::init)
