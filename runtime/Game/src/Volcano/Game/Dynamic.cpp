//
//
#include <Volcano/Game/Dynamic.hpp>

VOLCANO_GAME_BEGIN

Dynamic::Dynamic()
	: bt_dispatcher_(&bt_config_)
	, bt_world_(&bt_dispatcher_, &bt_broadphase_, &bt_solver_, &bt_config_)
	, gravity_(0.0f, -9.8f, 0.0f) {
	bt_world_.setGravity({ 0.0f, -9.8f, 0.0f });
}

void Dynamic::update(Clock::duration elapsed) {
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    auto elapsed_ms_double = double(elapsed_ms) / 1000;
    bt_world_.stepSimulation(elapsed_ms_double);
}

VOLCANO_GAME_END
