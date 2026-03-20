//
//
#include <Volcano/World/Dynamic.h>

VOLCANO_WORLD_BEGIN

Dynamic::Dynamic(QObject* parent)
    : QObject(parent)
    , enabled_(true)
    , gravity_(0.0f, -9.8f, 0.0f) {
}

void Dynamic::update(Clock::duration elapsed) noexcept {
    if (enabled_) {
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        // jph_physics_system_.Update(us / 1000000.0f);
    }
}

VOLCANO_WORLD_END
